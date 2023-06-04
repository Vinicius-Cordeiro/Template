// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Characters/Animations/Notifies/TemplateFootstepAnimNotify.h"

#include "Core/Characters/TemplateCharacter.h"
#include "Core/Data/Physics/TemplatePhysicalMaterial.h"
#include "Core/Data/Tags/TemplateGameplayTags.h"
#include "Core/Data/Effects/TemplateFootstepDataAsset.h"
#include "Core/Data/Effects/TemplateFootstepMapDataAsset.h"
#include "Core/Components/TemplateAbilitySystemComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "Niagara/Public/NiagaraComponent.h"

UTemplateFootstepAnimNotify::UTemplateFootstepAnimNotify() {
	static ConstructorHelpers::FObjectFinder<USoundBase> FootstepCUESound = TEXT("/Game/Template/Characters/Common/Audio/Footsteps/Foley_Footsteps/SCUE_Footsteps_Default.SCUE_Footsteps_Default");
	if (FootstepCUESound.Succeeded()) {
		FootstepDefaultSound = (FootstepCUESound.Object);
	}
}

void UTemplateFootstepAnimNotify::Notify(USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp,Animation,EventReference);

	if (!MeshComp || !Animation || (MeshComp && !MeshComp->GetOwner())) { return; }
	Character = Cast<ATemplateCharacter>(MeshComp->GetOwner());
	if (!Character) { return; }
	if (!Character->GetController()) { return; }
	if (!Character->GetFootstepDataAsset()) { return; }

	FHitResult HitResult;
	const FName TraceSocket { (Character->GetFootstepDataAsset()->FootstepSocket.ToString()) + (Character->GetFootstepDataAsset()->GetIsTracingRightFoot() ? FString("r") : FString("l")) };
	const FVector StartLocation { Character->GetMesh()->GetSocketLocation(TraceSocket) + (Character->GetActorUpVector() * 60) };
	const FVector EndLocation { StartLocation + (Character->GetActorUpVector() * -180) };
	const FCollisionQueryParams QueryParams { GetCollisionQueryParams(Character,true,true,true,EQueryMobilityType::Any) };

	if(bShouldDrawDebug){
		// Debug Line drawed for checking whether the trace did or did not hit the ground/surface below the character 
		DrawDebugLine(Character->GetWorld() , StartLocation , EndLocation , FColor::Red , false , 5.f , 1.f , 1.f);
	}

	const bool bDidHit { Character->GetWorld()->SweepSingleByChannel(HitResult,StartLocation,EndLocation,FQuat::Identity,ECC_Visibility,FCollisionShape::MakeSphere(2.f),QueryParams) };

	// We have a valid hit ! 
	if (!bDidHit) {
		return;
	}

	EPhysicalSurface Surface { SurfaceType_Default };

	FGameplayTag TagRequest { FGameplayTag::RequestGameplayTag("Physics.PhysicalMaterial.DEFAULT" , false) };

	if (HitResult.PhysMaterial.Get()) {
		UTemplatePhysicalMaterial* PhysMat { Cast<UTemplatePhysicalMaterial>(HitResult.PhysMaterial.Get()) };
		Surface = UPhysicalMaterial::DetermineSurfaceType(PhysMat);
		if (PhysMat)
			TagRequest = PhysMat->Tag;
		else
			TagRequest = FGameplayTag::RequestGameplayTag("Physics.PhysicalMaterial.DEFAULT" , false);
	} else {
		TagRequest = FGameplayTag::RequestGameplayTag("Physics.PhysicalMaterial.DEFAULT" , false);
		Surface = SurfaceType_Default;
	}

	const UTemplateFootstepDataAsset* FootstepData { Character->GetFootstepDataAsset()->GetFootstepEffectByTag(TagRequest) };

	if (!FootstepData) {
		if (FootstepDefaultSound)
			UGameplayStatics::PlaySoundAtLocation(Character->GetWorld() , FootstepDefaultSound , Character->GetActorLocation() , 1.f , 1.f , 0.f);
		return;
	}

	FGameplayTag CrouchedGameplayTag { FGameplayTag::RequestGameplayTag("Tag.Crouching",false) };

	const float CrouchPhysicsVolumeDivider { (Character->GetAbilitySystemComponent()->GetGameplayTagCount(CrouchedGameplayTag) > 0 ? 2.f : 1.f) };
	const float CrouchPhysicsPitchDivider { (Character->GetAbilitySystemComponent()->GetGameplayTagCount(CrouchedGameplayTag) > 0 ? 1.5f : 1.f) };

	const float PhysicsVolume { static_cast<float>(FMath::GetMappedRangeValueClamped(FVector2D(0.f,600.f),FootstepData->FootstepVolume,FMath::Abs(Character->GetVelocity().Size())) / CrouchPhysicsVolumeDivider) } ;
	const float PhysicsPitch { static_cast<float>(FMath::GetMappedRangeValueClamped(FVector2D(0.f,600.f), FootstepData->FootstepPitch, FMath::Abs(Character->GetVelocity().Size())) / CrouchPhysicsPitchDivider) };

	Character->GetFootstepAudio()->SetSound(FootstepData->FootstepSoundSFX);
	Character->GetFootstepAudio()->SetVolumeMultiplier(PhysicsVolume);
	Character->GetFootstepAudio()->SetPitchMultiplier(PhysicsPitch);
	Character->GetFootstepAudio()->Play();

	const float NiagaraSizeDivider { CrouchPhysicsVolumeDivider };
	// Necessary to static cast to float as unreal engine FMath range function now is templated 
	const float NiagaraParticleSize { static_cast<float>(FMath::GetMappedRangeValueClamped(FVector2D(0.f, 600.f), FVector2D(0.f, 1.f), FMath::Abs(Character->GetVelocity().Size()))) / NiagaraSizeDivider };
	// Spawn footstep niagara effect
	UNiagaraComponent* UNC = UNiagaraFunctionLibrary::SpawnSystemAttached(FootstepData->NiagaraParticleFX , Character->GetMesh() , TraceSocket , HitResult.Location , HitResult.ImpactNormal.ToOrientationRotator() , EAttachLocation::KeepWorldPosition , true);
	if (UNC) {
		UNC->SetWorldScale3D(FVector(NiagaraParticleSize)); // Resize the particle based on the character velocity 
	}

	// TODO Footprint DECAL 
	//...
	//...
	//...

	// Alternate between right foot and left foot 
	Character->GetFootstepDataAsset()->SetTracingFoot(!Character->GetFootstepDataAsset()->GetIsTracingRightFoot());

}
