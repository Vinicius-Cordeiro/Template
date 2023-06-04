// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TemplateFootstepMapDataAsset.generated.h"


class UTemplateFootstepDataAsset;
struct FGameplayTag;

USTRUCT(BlueprintType)
struct FFootstepMapStructure {
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere , Category = FootstepProperties)
	FGameplayTag Tag { FGameplayTag::RequestGameplayTag("Tag.PhysicalMaterials.DEFAULT") };

	UPROPERTY(EditAnywhere , Category = FootstepProperties)
	const UTemplateFootstepDataAsset* FootstepData { nullptr };

	FFootstepMapStructure() {
	}

};

UCLASS()
class TEMPLATE_API UTemplateFootstepMapDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:

	UTemplateFootstepMapDataAsset();

	UPROPERTY()
	UTemplateFootstepDataAsset* DefaultFootstep;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Footsteps" , meta = (ForceInlineRow , TitleProperty = "Tag"))
	TArray<FFootstepMapStructure>  FootstepEffectMap;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Footsteps")
	bool bIsTracingRightFoot { true };

	/** Bone Socket, Where the trace for footstep will look for collisions , as well as define on which socket the footstep effect will be attached to..
	*	FootstepSocket : foot_r or foot_l
	*/
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Footsteps")
	FName FootstepSocket { FName("foot_") };

#if WITH_EDITOR
	/**
	 * Called when a property on this object has been modified externally
	 *
	 * @param PropertyChangedEvent the property that was modified
	 */
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/** Get the Footstep fx with the tag
	* @param Tag  for recovering the effect to use
	* @return return the effect mapped with the passed tag..
	*/
	const UTemplateFootstepDataAsset* GetFootstepEffectByTag(const FGameplayTag& Tag);

	UFUNCTION(BlueprintCallable , Category = "Footsteps")
	void SetTracingFoot(const bool& TracingRightFoot) { bIsTracingRightFoot = !TracingRightFoot; }

	UFUNCTION(BlueprintPure , Category = "Footsteps")
	bool GetIsTracingRightFoot() { return bIsTracingRightFoot; }

};
