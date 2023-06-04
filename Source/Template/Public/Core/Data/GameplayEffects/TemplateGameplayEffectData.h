// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags/Classes/GameplayTagContainer.h"
#include "TemplateGameplayEffectData.generated.h"


struct FGameplayTag;

USTRUCT(BlueprintType)
struct FTemplateGameplayEffectsMap {
	GENERATED_BODY()
public:
	/*const FGameplayTag& , TSubclassOf<class UCTCGameplayEffect*/

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ability System")
	FGameplayTag GameplayTag;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ability System")
	TSubclassOf<class UTemplateGameplayEffect> GameplayEffectClass;

};


/**
 * 
 */
UCLASS()
class TEMPLATE_API UTemplateGameplayEffectData : public UDataAsset
{
	GENERATED_BODY()
public:
	/** Gameplay Effects map tagged for use with ASC tag system */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ability System" , meta = (ShowOnlyInnerProperties))
	TArray<FTemplateGameplayEffectsMap> GameplayEffectsMap;

	/** Initial Gameplay effects that loops on the character such as stamina recover, hunger cost , thirst cost, fatigue cost... */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Ability System")
	TArray<TSubclassOf<class UTemplateGameplayEffect>> InitialGameplayEffectClasses;

	/** Get the subclass of the gameplay effect with the tag
	* @param Tag  for recovering the gameplay effect to use
	* @return return the gameplay effect mapped with the passed tag..
	*/
	UFUNCTION(BlueprintPure , Category = "Ability System")
	TSubclassOf<class UTemplateGameplayEffect> GetGameplayEffectByTag(const FGameplayTag& Tag);
};
