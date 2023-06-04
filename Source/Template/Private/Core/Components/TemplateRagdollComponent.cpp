// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#include "Core/Components/TemplateRagdollComponent.h"

// Engine Includes

// Custom Includes
#include "Core/Characters/TemplateCharacter.h"
#include "Core/Characters/Animations/Instances/TemplateAnimInst.h"
#include "Core/Components/TemplateAbilitySystemComponent.h"
#include "Core/Data/Attributes/TemplateStatsAttributesSet.h"
#include "Core/Data/Attributes/TemplateMovementAttributesSet.h"
#include "Core/Data/Tags/TemplateGameplayTags.h"
#include "Core/Data/GameplayEffects/TemplateGameplayEffect.h"

// Sets default values for this component's properties
UTemplateRagdollComponent::UTemplateRagdollComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UTemplateRagdollComponent::BeginPlay() {
	Super::BeginPlay();

	// ...

	Character = Cast<ATemplateCharacter>(GetOwner());
	if (!Character) return;
	if (!Character->GetAbilitySystemComponent()) return;
	ASC = Cast<UTemplateAbilitySystemComponent>(Character->GetAbilitySystemComponent());
	if (!ASC) return;
	ASC->ApplyModToAttributeUnsafe(UTemplateMovementAttributesSet::GetDamageHeightThresholdAttribute() , EGameplayModOp::Override , FallDamageThresholdHeight);

}

// Called every frame
void UTemplateRagdollComponent::TickComponent(float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime , TickType , ThisTickFunction);
	// Try to get the valid owner of this component of type Character 
	if (!Character && GetOwner()) {
		Character = Cast<ATemplateCharacter>(GetOwner());
		if (!Character) return;
		if (!Character->GetAbilitySystemComponent()) return;
		ASC = Character->GetAbilitySystemComponent();
		if (!ASC) { GEngine->AddOnScreenDebugMessage(0 , DeltaTime , FColor::Red , "ASC Could not be found / initialized !"); return; }
	}

	// only check the velocity of the character if it's falling 
	if ((Character) && Character->GetCharacterMovement()->IsFalling())
		FallDistance = GetFallDistance();

	ASC->ApplyModToAttributeUnsafe(UTemplateMovementAttributesSet::GetZVelocityAttribute() , EGameplayModOp::Override , FMath::Abs(FallDistance));

	UpdateRagdollState();
	UpdateCapsuleLocation();

	// ...
	// ...
}


void UTemplateRagdollComponent::SimulateRagdoll() {
	if (!Character) return;
	if (!ASC) return;
	// we aren't allowed to ragdoll debug , if we are already trying to stand up.
	if (ASC->GetGameplayTagCount(GameplayTags.Tag_Standing) >= 1) return;
	// Enter free fall mode and start updating the capsule location
	Character->GetMesh()->SetSimulatePhysics(true);

	// Cancel all abilities and warn the gameplay tag system that the character is currently ragdolling and should not interact with the inputs
	ASC->SetLooseGameplayTagCount(GameplayTags.Tag_Ragdoll , 1);
	ASC->SetLooseGameplayTagCount(GameplayTags.Tag_Standing , 0);

	// Clear timer for standing up, if already set
	if (Character->GetWorldTimerManager().IsTimerActive(TH_RagdollRecover)) Character->GetWorldTimerManager().ClearTimer(TH_RagdollRecover);

	// Call the ragdoll recover function, where the animation is played based on the direction the character is facing
	Character->GetWorldTimerManager().SetTimer(TH_RagdollRecover , this , &UTemplateRagdollComponent::RagdollRecover , RagdollRecoverDelayTime , false);
}

void UTemplateRagdollComponent::OnLanded(const FHitResult& Hit) {
	// The character just finished landing 
	if (!Character || !ASC) { GEngine->AddOnScreenDebugMessage(-1 , 20.f , FColor::Orange , "Character or ASC could not be reached!!!"); return; }

	// Here we should apply damage to the character based on it's velocity when hit the ground
	// Get the time of fall from the character 
	const float FallTime = GetFallVelocity();

	// Get the fall distance 
	FallDistance = GetFallDistance();

	if (GEngine && ShowTraceDebug)
		GEngine->AddOnScreenDebugMessage(-1 , 10.f , FColor::Transparent , __FUNCTIONW__);

	// Play hard landing animation if the character is not ragdolling, and the height of damage for hard landing is enough
	if (ASC->GetGameplayTagCount(GameplayTags.Tag_Ragdoll) <= 0 && FallTime > FreeFallEnterTime && FallDistance < HardLandingEnterDistance) {
		if (GEngine && ShowTraceDebug)
			GEngine->AddOnScreenDebugMessage(-1 , 10.f , FColor::Red , "Should play hard landing montage!");
		bIsHardLanding = true;
		// lock the character movement
		Character->GetController()->SetIgnoreMoveInput(true);
		float AnimationTime = 0.f;
		if (HardLandingAnimMontage) {
			// play hard landing animation
			AnimationTime = Character->GetAnimInst()->Montage_Play(HardLandingAnimMontage , 1.f);
		}

		if (AnimationTime == 0.f) {
			// After some time reset the movement of the character 
			Character->GetWorldTimerManager().SetTimer(TH_RecoverHandLanding , this , &UTemplateRagdollComponent::HardLandingRecover , 1.f , false);
		}

	}
	if (FallDistance < FallDamageVelocity) {
		// Apply damage to the character 
		// Get the damage value based on D = FD (Fall Distance) / DTH (Damage Threshold Height)
		// Apply fall damage gameplay effect
		if (FallDamageGameplayEffect) {
			const FGameplayEffectContextHandle NewHandle = ASC->MakeEffectContext();
			const FGameplayEffectSpecHandle GameplayEffectSpecHandle = ASC->MakeOutgoingSpec(FallDamageGameplayEffect , 1 , NewHandle);

			ASC->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get() , ASC);
		} else {
			if (GEngine && ShowTraceDebug)
				GEngine->AddOnScreenDebugMessage(-1 , 10.f , FColor::Red , "Fall Damage Gameplay Effect Is Not Valid!!");
		}
		if (GEngine && ShowTraceDebug)
			GEngine->AddOnScreenDebugMessage(-1 , 10.f , FColor::Red , "Landed -> Character Should take damage : "  __FUNCTION__);
		//UGameplayStatics::ApplyDamage(Character,FMath::Abs(FallDistance / RagdollStructure.DamageThreshold),Character->GetInstigatorController(),Character,RagdollStructure.DefaultFallDamageType);
	}

	// Clear timer for standing up, if already set
	if (Character->GetWorldTimerManager().IsTimerActive(TH_RagdollRecover)) Character->GetWorldTimerManager().ClearTimer(TH_RagdollRecover);

	// Call the ragdoll recover function, where the animation is played based on the direction the character is facing
	if ((Character->IsAlive()) && ASC->GetGameplayTagCount(GameplayTags.Tag_Ragdoll) >= 1) {
		Character->GetWorldTimerManager().SetTimer(TH_RagdollRecover , this , &UTemplateRagdollComponent::RagdollRecover , RagdollRecoverDelayTime , false);
	}
}

float UTemplateRagdollComponent::GetFallDistance() const {

	return (Character) ? (Character->GetVelocity().Z * 9.8f) : 0.f;
}

float UTemplateRagdollComponent::GetFallVelocity() const {

	return ((Character) && Character->GetCharacterMovement()->IsFalling()) ? (Character->GetVelocity().Z / 9.8f) : 0.f;
}

void UTemplateRagdollComponent::UpdateRagdollState() {

	/** Check if the character refence is valid and if it is check if the character is currently falling, only then update the current fall velocity, only when the character is falling
	 * or if the ragdoll is currently deactivated for this -owner-
	 */
	if (!Character || (Character && !Character->GetCharacterMovement()->IsFalling())) return;
	if (!ASC) return;

	const float FallTime = GetFallVelocity();

	// Check if we are on the allowed velocity to start free falling and we are not already simulating physics
	if (FallTime < FreeFallEnterTime && !Character->GetMesh()->IsSimulatingPhysics()) {
		// Enter free fall mode and start updating the capsule location
		Character->GetMesh()->SetSimulatePhysics(true);

		// Cancel all abilities and warn the gameplay tag system that the character is currently ragdolling and should not interact with the inputs
		ASC->SetLooseGameplayTagCount(GameplayTags.Tag_Ragdoll , 1);
		ASC->SetLooseGameplayTagCount(GameplayTags.Tag_Standing , 0);
	}
}

void UTemplateRagdollComponent::UpdateCapsuleLocation() {

	if (!Character) return;
	if (!ASC) return;
	bool bWasAttributeFound { false };
	if (ASC->GetGameplayAttributeValue(UTemplateStatsAttributesSet::GetHealthAttribute() , bWasAttributeFound) <= 0.f) { return; }
	if (ASC->GetGameplayTagCount(GameplayTags.Tag_Ragdoll) > 0) {
		GetNewMeshLocation(MeshLocationFixed);
		/** Set the location of the capsule component, to be the one */
		Character->GetCapsuleComponent()->SetWorldLocation(MeshLocationFixed);
	} else {
		const FVector SocketLocation = Character->GetMesh()->GetSocketLocation(RagdollTraceSocket);
		/** Set the mesh location to be the one on the socket location, and add offset to match the capsule height */
		MeshLocationFixed = FVector(SocketLocation.X , SocketLocation.Y , SocketLocation.Z + RagdollZOffset);
	}
}

void UTemplateRagdollComponent::GetNewMeshLocation(FVector& OutMeshLocation) {

	// Only continues if the character is valid and the character is not already standing up 
	if (!Character) return;
	if (!ASC || (ASC && ASC->GetGameplayTagCount(GameplayTags.Tag_Standing) > 0)) return;

	// Trace to find the new location for the mesh
	FHitResult Hit;
	const FVector MeshSocketLocation = Character->GetMesh()->GetSocketLocation(RagdollTraceSocket);
	const FVector StartLocation = MeshSocketLocation;
	const FVector EndLocation = FVector(StartLocation.X , StartLocation.Y , StartLocation.Z - FallTraceDistance);

	FCollisionQueryParams QueryParams = GetCollisionQueryParams(Character , true , true , true , EQueryMobilityType::Any);

	// check if the character has hit a valid ground using the visibility channel to adjust the mesh location 
	const bool bHasHit = Character->GetWorld()->LineTraceSingleByChannel(Hit , StartLocation , EndLocation , ECC_Visibility , QueryParams);

	if (ShowTraceDebug) {
		DrawDebugDirectionalArrow(Character->GetWorld() , StartLocation , EndLocation , 4.f , FColor::Red , false , 5.f , 1 , 1.f);

		DrawDebugSphere(Character->GetWorld() , bHasHit
						? FVector(Hit.Location.X , Hit.Location.Y , Hit.Location.Z + RagdollZOffset)
						: FVector(MeshSocketLocation.X , MeshSocketLocation.Y , MeshSocketLocation.Z +
						RagdollZOffset) , 18.f , 3 , FColor::Orange , false , Character->GetWorld()->GetDeltaSeconds() , 2 , 1.f);
	}
	// Get the position based on if we hit or not the ground 
	const FVector NewCapLocation = bHasHit
		? FVector(Hit.Location.X , Hit.Location.Y , Hit.Location.Z + RagdollZOffset)
		: FVector(MeshSocketLocation.X , MeshSocketLocation.Y , MeshSocketLocation.Z + RagdollZOffset);

	// set our new mesh location
	OutMeshLocation = FMath::VInterpTo(OutMeshLocation , NewCapLocation , Character->GetWorld()->GetDeltaSeconds() , MeshFixInterpolationSpeed);

}

void UTemplateRagdollComponent::RagdollRecover() {
	// Check if the character is valid, and if the current posture of the character is ragdoll state and if the character is not already standing up 
	if (!Character) return;
	if (!ASC) return;
	if (GEngine && ShowTraceDebug)
		GEngine->AddOnScreenDebugMessage(-1 , 10.f , FColor::Red , "Ragdoll Recover Called Char and ASC Valid !");


	if (ASC->GetGameplayTagCount(GameplayTags.Tag_Ragdoll) <= 0) return;
	if (ASC->GetGameplayTagCount(GameplayTags.Tag_Standing) > 0) return;

	if (GEngine && ShowTraceDebug)
		GEngine->AddOnScreenDebugMessage(-1 , 10.f , FColor::Red , __FUNCTIONW__);

	Character->GetMesh()->SnapshotPose(RagdollSnapshot);
	Character->GetAnimInst()->SetRagdollSnapshot(RagdollSnapshot);

	// The character is currently standing up, this blocks the capsule position from being fixed, which would glitch and make the character levitate while standing up
	ASC->SetLooseGameplayTagCount(GameplayTags.Tag_Standing , 1);

	// Stop the mesh from simulating physics as we are now using the snapshot pose
	Character->GetMesh()->SetSimulatePhysics(false);

	// Check if the character is currently facing upwards or downwards, this basically selects which standing up animation should be played based on the way the character is facing 
	const bool bIsFacingUp = FVector::DotProduct(FQuat(Character->GetMesh()->GetSocketRotation(RagdollTraceSocket)).GetRightVector() , FVector(0 , 0 , 1)) > 0.f
		? true
		: false;

	const FVector StartLocationDebug = Character->GetMesh()->GetSocketLocation(RagdollTraceSocket);
	const FVector EndLocationDebug = StartLocationDebug + (FQuat(Character->GetMesh()->GetSocketRotation(RagdollTraceSocket)).GetRightVector() * 100.f);

	if (ShowTraceDebug)
		DrawDebugDirectionalArrow(Character->GetWorld() , StartLocationDebug , EndLocationDebug , 2.f , FColor::Red , false , 5.f , 1 , 1.f);

	// Set the new animation montage based on which way the character is facing, if facing up, that means the character has it's back facing the ground, so we play the ragdoll recover back montage
	UAnimMontage* StandingAnimationMontage = bIsFacingUp ? StandupBackAnimMontage : StandupFrontAnimMontage;

	// We need to have a  valid montage to continue and a valid controller as we ignore move input, while we are standing up
	if (!StandingAnimationMontage || !Character->GetController()) return;

	Character->GetController()->SetIgnoreMoveInput(true);
	FallDistance = 0.f;

	ASC->ApplyModToAttributeUnsafe(UTemplateStatsAttributesSet::GetDamageAttribute() , EGameplayModOp::Override , 0.f);
	ASC->ApplyModToAttributeUnsafe(UTemplateMovementAttributesSet::GetZVelocityAttribute() , EGameplayModOp::Override , 0.f);
	// TODO Send gameplay event as the character is standing up to block other interactions while doing it so..
	// Cancel all abilities and warn the gameplay tag system that the character is currently standing and should not interact with the inputs and remove previous ragdoll state tag 
	ASC->SetLooseGameplayTagCount(GameplayTags.Tag_Standing , 1);
	ASC->SetLooseGameplayTagCount(GameplayTags.Tag_Ragdoll , 0);
	// Play the animation montage, which will on a notify, call a function to enable the movement of the character again and remove the tags for ragdoll as well as set the character to not being standing up to avoid movement
	Character->PlayAnimMontage(StandingAnimationMontage);
}

void UTemplateRagdollComponent::HardLandingRecover() {
	if (!Character) { return; }
	if (!Character->GetController()) { return; }
	if (!ASC) { return; }
	ASC->ApplyModToAttributeUnsafe(UTemplateStatsAttributesSet::GetDamageAttribute() , EGameplayModOp::Override , 0.f);
	ASC->ApplyModToAttributeUnsafe(UTemplateMovementAttributesSet::GetZVelocityAttribute() , EGameplayModOp::Override , 0.f);
	// TODO Send gameplay event as the character is standing up to block other interactions while doing it so..
	// Cancel all abilities and warn the gameplay tag system that the character is currently standing and should not interact with the inputs and remove previous ragdoll state tag 
	ASC->SetLooseGameplayTagCount(GameplayTags.Tag_Standing , 0);
	ASC->SetLooseGameplayTagCount(GameplayTags.Tag_Ragdoll , 0);
	ASC->SetLooseGameplayTagCount(GameplayTags.Tag_Falling , 0);
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	Character->GetController()->ResetIgnoreInputFlags();
	bIsHardLanding = false;
}

