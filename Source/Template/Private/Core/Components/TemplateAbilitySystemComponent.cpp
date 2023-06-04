// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Components/TemplateAbilitySystemComponent.h"


#include "AbilitySystemBlueprintLibrary.h"

void UTemplateAbilitySystemComponent::OnTagUpdated(const FGameplayTag& Tag , bool TagExists) {
	Super::OnTagUpdated(Tag , TagExists);

	if (GetGameplayTagCount(Tag) > 0) return;

	const FGameplayTagContainer Container { FGameplayTag::RequestGameplayTag(Tag.GetTagName()) };
	RemoveActiveEffectsWithTags(Container);
}

void UTemplateAbilitySystemComponent::MarkActiveGameplayEffectDirty(FActiveGameplayEffect* ActiveGE) {
	if (!ActiveGE) return;
	ActiveGameplayEffects.MarkItemDirty(*ActiveGE);
}

void UTemplateAbilitySystemComponent::CheckActiveEffectDuration(const FActiveGameplayEffectHandle& Handle) {
	ActiveGameplayEffects.CheckDuration(Handle);
}

FActiveGameplayEffect* UTemplateAbilitySystemComponent::GetActiveGameplayEffect_Mutable(const FActiveGameplayEffectHandle Handle) {
	return ActiveGameplayEffects.GetActiveGameplayEffect(Handle);
}

TArray<FActiveGameplayEffectHandle> UTemplateAbilitySystemComponent::GetAllActiveEffectHandles() const {
	return ActiveGameplayEffects.GetAllActiveEffectHandles();
}
