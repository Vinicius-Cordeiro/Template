// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Data/Interfaces/TemplateInterface.h"
#include "TemplateRagdollComponent.generated.h"


class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEMPLATE_API UTemplateRagdollComponent : public UActorComponent , public ITemplateInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTemplateRagdollComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	UPROPERTY()
	TObjectPtr<class ATemplateCharacter> Character;

	UPROPERTY()
	TObjectPtr<class UTemplateAbilitySystemComponent> ASC;

	/** Debug function to simulate ragdoll state */
	virtual void SimulateRagdoll();
	/** Landed Implementation for when the character land , called from the base character */
	UFUNCTION()
	virtual void OnLanded(const FHitResult& Hit);
	/** Get the character current falling distance d = v * g */
	virtual float GetFallDistance() const;
	/** Get the character current falling time t = v / g */
	virtual float GetFallVelocity() const;

	/** Function to update the ragdoll state to enter ragdoll , based on the distance which the character has traveled downwards , and speed
	*	as well as remove tags, and add , such as add the ragdoll tag, and remove the standing up tag from the ASC
	*	Function also used for predicting the landing of the character
	*/
	UFUNCTION()
	virtual void UpdateRagdollState();
	/** Update the capsule location to the new location, usually the Main Mesh Pelvis location if on free fall else the own capsule location reset */
	virtual void UpdateCapsuleLocation();
	/** Traces looking to find a new location for the mesh , and capsule using a socket on the character mesh, tracing downwards */
	virtual void GetNewMeshLocation(FVector& OutMeshLocation);
	/** Function to start the recover of the character to stand up after free falling */
	virtual void RagdollRecover();
	/** Function fallback so the character can recover after hard landing */
	virtual void HardLandingRecover();
protected:

	FVector MeshLocationFixed { FVector(0,0,90) };

	/** Socket which the trace to check whether the character is with it's back on the floor or it's front which defines the animation for standing up */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	FName RagdollTraceSocket { FName("RagdollTrace_Socket") };

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	bool bIsHardLanding { false };

	/** The speed the character has to be to enter in free fall mode */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	float FreeFallEnterTime { -100 };

	/** The velocity which the character will have a hard landing animation */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	float HardLandingEnterDistance { -4802.f };

	/** The velocity in which the character will start taking fall damage */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	float FallDamageVelocity { -4483.f };

	/** Damage threshold height to clamp the damage the character will take when falling */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	float FallDamageThresholdHeight { 20000.f };

	/** Distance trace downwards to find the new mesh location , and fix the capsule location */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	float FallTraceDistance { 180.f };

	/** Offset of the capsule from the mesh , when reset */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	float RagdollZOffset { 90.f };

	/** Ragdoll Recover delay time */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	float RagdollRecoverDelayTime { 5.f };

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	float MeshFixInterpolationSpeed { 4.f };

	/** Variable of debug to show or not the debug prints and geometry on the game world */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	uint8 ShowTraceDebug { false };

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	FPoseSnapshot RagdollSnapshot;

	/** Montage for the character hard landing */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	TObjectPtr<UAnimMontage> HardLandingAnimMontage;

	/** Animation montage of the character standing up from it's front */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	TObjectPtr<UAnimMontage> StandupFrontAnimMontage;

	/** Animation Montage of the character standing up from it's back */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	TObjectPtr<UAnimMontage> StandupBackAnimMontage;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ragdoll", meta=(AllowPrivateAccess=true))
	TSubclassOf<class UTemplateGameplayEffect> FallDamageGameplayEffect;

	// Timer for ragdoll recover 
	FTimerHandle TH_RagdollRecover;
	FTimerHandle TH_RecoverHandLanding;

friend UTemplateMovementAttributesSet;
friend ATemplateCharacter;

	float FallDistance { 0.f };

};
