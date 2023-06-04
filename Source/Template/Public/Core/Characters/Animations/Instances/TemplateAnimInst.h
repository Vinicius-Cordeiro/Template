// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TemplateAnimInst.generated.h"

class ATemplateCharacter;
class UTemplateAbilitySystemComponent;

/**
 *
 */
UCLASS()
class UTemplateAnimInst : public UAnimInstance
{
	GENERATED_BODY()
public:
	UTemplateAnimInst();
private:

	/** Stored pointer for the character that this instance is applied to the mesh */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	TObjectPtr<ATemplateCharacter> Character { nullptr };

	/** Stored pointer of the ability system used by the character */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	TObjectPtr<UTemplateAbilitySystemComponent> ASC { nullptr };

	/** Bool to check if the character is currently moving */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	bool bIsMoving { false };

	/** Track for when the movement of the character should move or not  */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Animations" , meta = (AllowPrivateAccess = true))
	bool bShouldMove { false };

	/** Bool to check if the character is currently falling */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	bool bIsFalling { false };

	/** Bool to check whether the character is currently crouched */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Animations" , meta = (AllowPrivateAccess = true))
	bool bIsCrouching { false };

	/** Bool to check if the character is currently jumping, which will trigger the jump states */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	bool bIsJumping { false };

	/** Bool to check if the character is currently on ragdoll mode */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	bool bIsRagdollActive { false };

	/** Bool to check if the character is currently standing which will apply the pose snapshot for the ragdoll */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	bool bIsStandingUp { false };

	/** Bool to check if the character is currently turning right */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	bool bTurnRight { false };

	/** Bool to check if the character is currently turning left */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	bool bTurnLeft { false };

	/** Bool to check whether the character animations should be mirrored from right to left and vice-versa */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	bool bShouldMirror { false };

	/** Bool to check if the player is currently using the orbit character func which will stop turn animations, */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	bool bIsOrbitingCharacter { false };

	/** Bool to check whether the Pitch value from the input should be inverted or not */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Animations" , meta = (AllowPrivateAccess = true))
	bool bInvertPitchValue { true };

	/** Bool to check whether the animations should use aim offsets or not */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Animations" , meta = (AllowPrivateAccess = true))
	bool bShouldUseAimOffset { false };

	/** Forward velocity of the character used to apply the animations on the character, with the animation blendspaces */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	float ForwardVelocity { 0.f };

	/** Right velocity of the character used to apply the animations on the character, with the animation blendspaces */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	float RightVelocity { 0.f };

	/** Yaw offset value of the character controller rotation used to blend the aim offset based on the rotation */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	float YawOffset { 0.f };

	/** Pitch offset value of the character controller rotation used to blend the aim offset based on the rotation */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	float PitchOffset { 0.f };

	/** Turn Delta used to animate the character turning after a certain threshold is hit */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	float TurnOffsetDelta { 0.f };
	float LastFrameTurnOffset { 0.f };

	/** How fast the turn offset delta will blend from the last frame to the current frame */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	float TurnOffsetInterpolationSpeed { 4.f };

	/** Alpha of the Breathing Additive used to show if the character is tired or not */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	float BreathingAdditiveAlpha { 0.f };

	/** Landing additive alpha used to blend when the character just landed , to apply a certain down force to the character body */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = Animations , meta = (AllowPrivateAccess = true))
	float LandingAdditiveAlpha { 0.f };


	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Animations" , meta = (AllowPrivateAccess = true))
	float PreviousFrameYaw { 0.f };

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Animations" , meta = (AllowPrivateAccess = true))
	float DeltaYawInterpolationSpeed { 1.f };

	/** Alpha for the additive animations for breathing Heavy */
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Animations" , meta = (AllowPrivateAccess = true))
	float BreathAdditiveAlpha { 0.f };

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Animations" , meta = (AllowPrivateAccess = true))
	float FallingVelocity { 0.f };

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = Animations , meta =(AllowPrivateAccess = true))
	FVector Acceleration { FVector(0.f) }; 

	FVector PreviousAcceleration { FVector(0.f) };


	/** Ground velocity of the character */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Animations" , meta = (AllowPrivateAccess = true))
	FVector Velocity { FVector() };

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Animations" , meta = (AllowPrivateAccess = true))
	FPoseSnapshot RagdollSnapshot;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Animations" , meta = (AllowPrivateAccess = true))
	FPoseSnapshot DeathSnapshot;


	friend ATemplateCharacter;

public:
	// Native initialization override point
	virtual void NativeInitializeAnimation() override;
	// Native update override point. It is usually a good idea to simply gather data in this step and 
	// for the bulk of the work to be done in NativeThreadSafeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	// Native thread safe update override point. Executed on a worker thread just prior to graph update 
	// for linked anim instances, only called when the hosting node(s) are relevant
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	// Executed when begin play is called on the owning component
	virtual void NativeBeginPlay() override;

	/** Executed to initialize the default values for the animation variables */
	virtual void InitializeAnimation();

	/** Executed to update the animations default values , that need to be acquired from the character or ASC */
	virtual void UpdateCharacterAnimation(const float & DeltaSeconds);

	virtual void SetRagdollSnapshot(const FPoseSnapshot& NewRagdollSnapshot);
};
