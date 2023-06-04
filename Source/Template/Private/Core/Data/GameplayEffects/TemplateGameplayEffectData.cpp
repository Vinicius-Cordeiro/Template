// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Data/GameplayEffects/TemplateGameplayEffectData.h"

#include "Core/Data/GameplayEffects/TemplateGameplayEffect.h"

TSubclassOf<class UTemplateGameplayEffect> UTemplateGameplayEffectData::GetGameplayEffectByTag(const FGameplayTag& Tag) {
	for (auto GameplayEffect : GameplayEffectsMap) {
		if (GameplayEffect.GameplayTag == Tag && GameplayEffect.GameplayEffectClass) {
			return GameplayEffect.GameplayEffectClass;
		}
	}
	return TSubclassOf<UTemplateGameplayEffect>();
}
