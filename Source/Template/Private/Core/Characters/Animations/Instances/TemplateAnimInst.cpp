// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Characters/Animations/Instances/TemplateAnimInst.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Core/Data/Tags/TemplateGameplayTags.h"
#include "Core/Characters/TemplateCharacter.h"
#include "Core/Components/TemplateAbilitySystemComponent.h"
#include "Core/Data/Attributes/TemplateMovementAttributesSet.h"
#include "Core/Data/Attributes/TemplateStatsAttributesSet.h"

UTemplateAnimInst::UTemplateAnimInst() {
}

void UTemplateAnimInst::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
	InitializeAnimation();
}

void UTemplateAnimInst::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	UpdateCharacterAnimation(DeltaSeconds);
}

void UTemplateAnimInst::NativeThreadSafeUpdateAnimation(float DeltaSeconds) {
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

void UTemplateAnimInst::NativeBeginPlay() {
	Super::NativeBeginPlay();
	InitializeAnimation();
}

void UTemplateAnimInst::InitializeAnimation() {
	if (!Character) { Character = Cast<ATemplateCharacter>(TryGetPawnOwner()); }
	if (Character && !ASC) { ASC = Cast<UTemplateAbilitySystemComponent>(Character->GetAbilitySystemComponent()); }
}

void UTemplateAnimInst::UpdateCharacterAnimation(const float& DeltaSeconds) {

	if (!Character) { Character = Cast<ATemplateCharacter>(TryGetPawnOwner()); }
	if (Character && !ASC) { ASC = Cast<UTemplateAbilitySystemComponent>(Character->GetAbilitySystemComponent()); }
	if(!Character || !ASC) return;
	
	const FTemplateGameplayTags& GameplayTags { FTemplateGameplayTags::Get() };

	// Forward velocity of the character
	ForwardVelocity = FVector::DotProduct(Character->GetVelocity() , Character->GetActorForwardVector());
	// Get the right velocity of the character based on whether it's mirrored animation or not by change shoulder input 
	RightVelocity = FVector::DotProduct(Character->GetVelocity() , Character->GetActorRightVector()) * (Character->GetAbilitySystemComponent()->GetGameplayTagCount(GameplayTags.Tag_MirrorAnimations) > 0 ? -1.f : 1.f);
	// Get character aim offset from the controller based on the mouse/gamepad input 
	PitchOffset = UKismetMathLibrary::NormalizedDeltaRotator(Character->GetActorRotation() , Character->GetControlRotation()).Pitch * (bInvertPitchValue ? -1.f : 1.f);
	// Get the aim offset yaw rotation based on if the character is currently mirrored or not, otherwise when the shoulder change the aim offset will be  incorrect 
	YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(Character->GetActorRotation() , Character->GetControlRotation()).Yaw * (Character->GetAbilitySystemComponent()->GetGameplayTagCount(GameplayTags.Tag_MirrorAnimations) > 0 ? -1.f : 1.f);
	// Check if the character is currently moving, alternatively this could use the gameplay tags, as it perform the same check, so it would be optimal , but now is not the time... 
	bIsMoving = FMath::Abs(Character->GetVelocity().Size()) > 10.f ? true : false;
	// Get whether the character should move or not , this take in consideration the current acceleration of the character and velocity.. 
	bShouldMove = Character->GetCharacterMovement()->GetCurrentAcceleration() != FVector(0.f) * 0.f && UKismetMathLibrary::VSizeXY(Character->GetMovementComponent()->Velocity) > .3f ? true : false;
	// Check if the character is currently with the camera on the left or right shoulder, left meaning that animations should be mirrored.. 
	bShouldMirror = Character->GetAbilitySystemComponent()->GetGameplayTagCount(GameplayTags.Tag_MirrorAnimations) > 0;
	// Get if the character is currently crouching, used for blending between animations set 
	bIsCrouching = Character->GetAbilitySystemComponent()->GetGameplayTagCount(GameplayTags.Tag_Crouching) > 0;
	// Check if the character is currently falling..
	bIsFalling = Character->GetMovementComponent()->IsFalling();
	// Get character raw velocity for simple checks 
	Velocity = Character->GetMovementComponent()->Velocity;
	// Fall velocity to check weather the character should play additive animation to blend when landing 
	FallingVelocity = FMath::Abs(Character->GetVelocity().Z);
	// Check if the character should use aim offsets, if the camera is orbiting the character will not use aim offset 
	bShouldUseAimOffset = (Character->GetAbilitySystemComponent()->GetGameplayTagCount(GameplayTags.InputTag_OrbitCharacter) <= 0 ? true : false);
	// Use values between minus one and one , as the animation for turn in place at the moment is using only the 90 degree turn
	//Should check if the character is currently using the orbit camera to avoid weird animation !!!
	bTurnRight = (YawOffset > .25f ? (Character->GetAbilitySystemComponent()->GetGameplayTagCount(GameplayTags.InputTag_OrbitCharacter) <= 0 ? true : false) : false);
	bTurnLeft = (YawOffset < -.25f ? (Character->GetAbilitySystemComponent()->GetGameplayTagCount(GameplayTags.InputTag_OrbitCharacter) <= 0 ? true : false) : false);

	const float DeltaYaw { static_cast<float>(FMath::FInterpTo(PreviousFrameYaw, Character->GetControlRotation().Yaw, GetWorld()->GetDeltaSeconds(), DeltaYawInterpolationSpeed)) };
	PreviousFrameYaw = static_cast<float>(FMath::GetMappedRangeValueClamped(FVector2D(0.f , 360.f) , FVector2D(-180.f , 180.f) , DeltaYaw));

	bool bWasAttributeFound { false };

	BreathAdditiveAlpha = static_cast<float>(FMath::GetMappedRangeValueClamped(FVector2D(0.f, Character->GetAbilitySystemComponent()->GetGameplayAttributeValue(UTemplateStatsAttributesSet::GetMaxStaminaAttribute() , bWasAttributeFound)),FVector2D(1.f,0.f), Character->GetAbilitySystemComponent()->GetGameplayAttributeValue(UTemplateStatsAttributesSet::GetStaminaAttribute() , bWasAttributeFound)));
}

void UTemplateAnimInst::SetRagdollSnapshot(const FPoseSnapshot& NewRagdollSnapshot) {
	RagdollSnapshot = NewRagdollSnapshot;
}
