// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Data/Attributes/TemplateAttributesSet.h"

#include "Core/Characters/TemplateCharacter.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"


UTemplateAttributesSet::UTemplateAttributesSet() {
}

void UTemplateAttributesSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void UTemplateAttributesSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
	Super::PostGameplayEffectExecute(Data);


	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();

	UAbilitySystemComponent* SourceASC = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();


	float DeltaValue { 0.f };

	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive) {
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	ATemplateCharacter* TargetCharacter { nullptr };

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) {
		AActor* TargetActor { nullptr };

		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<ATemplateCharacter>(TargetActor);

	}

	// example of how to apply this function on existing child attributes 

	//if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
	//	SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

	//	if (TargetCharacter) {
	//		TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
	//	}
	//}

}

void UTemplateAttributesSet::PreAttributeChange(const FGameplayAttribute& Attribute , float& NewValue) {
	Super::PreAttributeChange(Attribute , NewValue);

	// example of how to apply this function on existing child attributes 
	//if (Attribute == GetMaxHealthAttribute()) {
	//	AdjustAttributeForMaxChange(Health,MaxHealth,NewValue,GetHealthAttribute());
	//}
}

void UTemplateAttributesSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute , const FGameplayAttributeData& MaxAttribute , float NewMaxValue , const FGameplayAttribute& AffectedAttributeProperty) const {
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue , NewMaxValue) && ASC) {
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		const float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		ASC->ApplyModToAttributeUnsafe(AffectedAttributeProperty , EGameplayModOp::Additive , NewDelta);
	}
}
