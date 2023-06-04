// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags/Classes/GameplayTagContainer.h"
#include "TemplateInputConfig.generated.h"

class UInputAction;
struct FGameplayTag;

/**
 * Tagged Input Action Map 
 */
USTRUCT(BlueprintType)
struct FTaggedInputAction
{
	GENERATED_BODY()
public:

	/** Input Action asset used to define the behavior when a input is pressed,triggered etc.. */
	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	/** Gameplay Tag used to map the input action */
	UPROPERTY(EditDefaultsOnly , Meta = (Categories = Tag))
	FGameplayTag Tag;
};


UCLASS()
class TEMPLATE_API UTemplateInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag) const;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , meta = (TitleProperty = "InputAction" , ShowOnlyInnerProperties))
	TArray<FTaggedInputAction> TaggedInputActions;

};
