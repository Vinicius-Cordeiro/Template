// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Data/Attributes/TemplateMovementAttributesSet.h"

#include "Core/Characters/TemplateCharacter.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"


UTemplateMovementAttributesSet::UTemplateMovementAttributesSet() {
}

void UTemplateMovementAttributesSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTemplateMovementAttributesSet , CurrentVelocity);
	DOREPLIFETIME(UTemplateMovementAttributesSet , MinVelocity);
	DOREPLIFETIME(UTemplateMovementAttributesSet , MaxVelocity);
	DOREPLIFETIME(UTemplateMovementAttributesSet , ZVelocity);
	DOREPLIFETIME(UTemplateMovementAttributesSet , DamageHeightThreshold);

}

void UTemplateMovementAttributesSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
	Super::PostGameplayEffectExecute(Data);

	const FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();

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


	/** Set Current Velociity of the character movement component*/
	if (Data.EvaluatedData.Attribute == GetCurrentVelocityAttribute()) {
		SetCurrentVelocity(FMath::Clamp(GetCurrentVelocity() , GetMinVelocity() , GetMaxVelocity()));

		if (TargetCharacter) {
			TargetCharacter->HandleMovementSpeedChange();
		}
	}

	if (Data.EvaluatedData.Attribute == GetZVelocityAttribute()) {
		SetZVelocity(GetZVelocity());
	}

	if (Data.EvaluatedData.Attribute == GetDamageHeightThresholdAttribute()) {
		SetDamageHeightThreshold(GetDamageHeightThreshold());
	}
}

void UTemplateMovementAttributesSet::PreAttributeChange(const FGameplayAttribute& Attribute , float& NewValue) {
	Super::PreAttributeChange(Attribute,NewValue);

	if (Attribute == GetMaxVelocityAttribute()) {
		AdjustAttributeForMaxChange(GetCurrentVelocity() , GetMaxVelocity() , NewValue , GetCurrentVelocityAttribute());
	}

}

void UTemplateMovementAttributesSet::OnRep_CurrentVelocity(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateMovementAttributesSet , CurrentVelocity , OldValue);
}

void UTemplateMovementAttributesSet::OnRep_MinVelocity(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateMovementAttributesSet , MinVelocity , OldValue);
}

void UTemplateMovementAttributesSet::OnRep_MaxVelocity(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateMovementAttributesSet , MaxVelocity , OldValue);
}

void UTemplateMovementAttributesSet::OnRep_DamageHeightThreshold(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateMovementAttributesSet , DamageHeightThreshold , OldValue);
}

void UTemplateMovementAttributesSet::OnRep_ZVelocity(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateMovementAttributesSet , ZVelocity , OldValue);
}
