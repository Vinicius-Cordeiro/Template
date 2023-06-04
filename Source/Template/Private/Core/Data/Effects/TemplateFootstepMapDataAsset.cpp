// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#include "Core/Data/Effects/TemplateFootstepMapDataAsset.h"

#include "Core/Data/Tags/TemplateGameplayTags.h"

UTemplateFootstepMapDataAsset::UTemplateFootstepMapDataAsset() {
	static ConstructorHelpers::FObjectFinder<UTemplateFootstepDataAsset> FootstepData = TEXT("/Game/Template/Core/Data/Effects/DA_FootstepData.DA_FootstepData");
	if (FootstepData.Succeeded()) {
		DefaultFootstep = (FootstepData.Object);
	}

	const FTemplateGameplayTags& GameplayTag { FTemplateGameplayTags::Get() };
	TArray<FGameplayTag> GameplayTagsMap;
	GameplayTagsMap.AddUnique(GameplayTag.Tag_DEFAULTPHYSMATERIAL);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_ACID);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_ASPHALT);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_BARK);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_BLOOD);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_BLOODCRITICAL);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_BONES);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_BRICK);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_BROKENGLASS);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_CARDBOARD);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_CARPET);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_CARBONFIBER);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_CERAMIC);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_CONCRETE);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_CORRUGATEDSTEEL);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_DIRT);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_DIRT2);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_ELECTRICAL);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_FABRIC);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_FENCE);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_GLASS);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_GRASS);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_GRAVEL);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_ICE);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_LEATHER);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_LEAVES);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_LIMESTONE);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_METAL);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_METALDULL);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_MUD);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_OIL);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_PAPER);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_PLASTIC);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_POLYSTYRENE);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_ROCK);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_ROOFTILES);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_RUBBER);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_SAND);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_SOIL);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_TILES);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_TRASH);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_VEST);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_VEGETATION);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_WATER);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_WATERHEAVY);
	GameplayTagsMap.AddUnique(GameplayTag.Tag_WOOD);

	for (FGameplayTag Tags : GameplayTagsMap) {
		FFootstepMapStructure NewFootstep;
		NewFootstep.Tag = Tags;
		NewFootstep.FootstepData = DefaultFootstep ? DefaultFootstep : nullptr;
		FootstepEffectMap.Add(NewFootstep);
		//if (FootstepEffectMap.IsValidIndex(i)) { if (FootstepEffectMap[i].Tag != Tags) { FootstepEffectMap.Add(NewFootstep); } }

	}

}
#if WITH_EDITOR
/**
 * Called when a property on this object has been modified externally
 *
 * @param PropertyChangedEvent the property that was modified
 */
void UTemplateFootstepMapDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif

/** Get the Footstep fx with the tag
* @param Tag  for recovering the effect to use
* @return return the effect mapped with the passed tag..
*/
const UTemplateFootstepDataAsset* UTemplateFootstepMapDataAsset::GetFootstepEffectByTag(const FGameplayTag& Tag) {

	for (const FFootstepMapStructure& FootstepMap : FootstepEffectMap) {
		if (FootstepMap.FootstepData && FootstepMap.Tag == Tag) {
			return FootstepMap.FootstepData;
		}
	}
	return nullptr;
}
