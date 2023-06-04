// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Core/Characters/Animations/Modifiers/TemplateAnimationModifier.h"
#include "TemplateFootstepAnimModifier.generated.h"

class UTemplateFootstepAnimNotify;

/**
 * 
 */
UCLASS()
class TEMPLATE_API UTemplateFootstepAnimModifier : public UTemplateAnimationModifier
{
	GENERATED_BODY()

public:
	/** Executed when the Animation is initialized (native event for debugging / testing purposes) */
	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;
	virtual void OnRevert_Implementation(UAnimSequence* AnimationSequence) override;

	/** Name of the track that will be created when this animation modifier is applied */
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite , Category = "Footstep")
	FName TrackName { "Footstep_Track" };

	/** Color of the notify track that will be added to the animation tracks */
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite , Category = "Footstep")
	FColor TrackColor { FColor::Silver };

	/** Class of the animation notify to be added to the animation sequence, on the track define previously */
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Footstep")
	TSubclassOf<UTemplateFootstepAnimNotify> NotifyClass { TSubclassOf<UTemplateFootstepAnimNotify>() };
};
