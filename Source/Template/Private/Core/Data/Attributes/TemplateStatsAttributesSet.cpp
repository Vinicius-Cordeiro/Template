// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Data/Attributes/TemplateStatsAttributesSet.h"

#include "Core/Characters/TemplateCharacter.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UTemplateStatsAttributesSet::UTemplateStatsAttributesSet() {
}

void UTemplateStatsAttributesSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTemplateStatsAttributesSet , Health);
	DOREPLIFETIME(UTemplateStatsAttributesSet , MaxHealth);
	DOREPLIFETIME(UTemplateStatsAttributesSet , Stamina);
	DOREPLIFETIME(UTemplateStatsAttributesSet , MaxStamina);
	DOREPLIFETIME(UTemplateStatsAttributesSet , Nutrition);
	DOREPLIFETIME(UTemplateStatsAttributesSet , MaxNutrition);
	DOREPLIFETIME(UTemplateStatsAttributesSet , Hydration);
	DOREPLIFETIME(UTemplateStatsAttributesSet , MaxHydration);
	DOREPLIFETIME(UTemplateStatsAttributesSet , Fatigue);
	DOREPLIFETIME(UTemplateStatsAttributesSet , MaxFatigue);
	DOREPLIFETIME(UTemplateStatsAttributesSet , Damage);
}

void UTemplateStatsAttributesSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
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

	// example of how to apply this function on existing child attributes 

	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
		SetHealth(FMath::Clamp(GetHealth() , 0.f , GetMaxHealth()));
		if (TargetCharacter) {
			TargetCharacter->HandleHealthChange(GetHealth());
		}
	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute()) {
		SetStamina(FMath::Clamp(GetStamina() , 0.f , GetMaxStamina()));
		if (TargetCharacter) {
			TargetCharacter->HandleStaminaChange(GetStamina());
		}
	}


	if (Data.EvaluatedData.Attribute == GetNutritionAttribute()) {
		SetNutrition(FMath::Clamp(GetNutrition() , 0.f , GetMaxNutrition()));
		if (TargetCharacter) {
		}
	}

	if (Data.EvaluatedData.Attribute == GetHydrationAttribute()) {
		SetHydration(FMath::Clamp(GetHydration() , 0.f , GetMaxHydration()));
		if (TargetCharacter) {
		}
	}

	if (Data.EvaluatedData.Attribute == GetFatigueAttribute()) {
		SetFatigue(FMath::Clamp(GetFatigue() , 0.f , GetMaxFatigue()));
		if (TargetCharacter) {
		}
	}


	if (Data.EvaluatedData.Attribute == GetDamageAttribute()) {
		SetDamage(GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - FMath::Abs(GetDamage()) , 0.f , GetMaxHealth()));
		if (TargetCharacter) {
			TargetCharacter->HandleHealthChange(GetHealth());
		}
	}

}

void UTemplateStatsAttributesSet::PreAttributeChange(const FGameplayAttribute& Attribute , float& NewValue) {
	Super::PreAttributeChange(Attribute , NewValue);

	if (Attribute == GetMaxHealthAttribute()) {
		AdjustAttributeForMaxChange(Health , MaxHealth , NewValue , GetHealthAttribute());
	}
	if (Attribute == GetMaxStaminaAttribute()) {
		AdjustAttributeForMaxChange(Stamina , MaxStamina , NewValue , GetStaminaAttribute());
	}
	if (Attribute == GetMaxNutritionAttribute()) {
		AdjustAttributeForMaxChange(Nutrition , MaxNutrition , NewValue , GetNutritionAttribute());
	}
	if (Attribute == GetMaxHydrationAttribute()) {
		AdjustAttributeForMaxChange(Hydration , MaxHydration , NewValue , GetHydrationAttribute());
	}
	if (Attribute == GetMaxFatigueAttribute()) {
		AdjustAttributeForMaxChange(Fatigue , MaxFatigue , NewValue , GetFatigueAttribute());
	}
}

void UTemplateStatsAttributesSet::OnRep_Health(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateStatsAttributesSet , Health , OldValue);
}

void UTemplateStatsAttributesSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateStatsAttributesSet , MaxHealth , OldValue);
}

void UTemplateStatsAttributesSet::OnRep_Stamina(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateStatsAttributesSet , Stamina , OldValue);
}

void UTemplateStatsAttributesSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateStatsAttributesSet , MaxStamina , OldValue);
}

void UTemplateStatsAttributesSet::OnRep_Nutrition(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateStatsAttributesSet , Nutrition , OldValue);
}

void UTemplateStatsAttributesSet::OnRep_MaxNutrition(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateStatsAttributesSet , MaxNutrition , OldValue);
}

void UTemplateStatsAttributesSet::OnRep_Hydration(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateStatsAttributesSet , Hydration , OldValue);
}

void UTemplateStatsAttributesSet::OnRep_MaxHydration(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateStatsAttributesSet , MaxHydration , OldValue);
}

void UTemplateStatsAttributesSet::OnRep_Fatigue(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateStatsAttributesSet , Fatigue , OldValue);
}

void UTemplateStatsAttributesSet::OnRep_MaxFatigue(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateStatsAttributesSet , MaxFatigue , OldValue);
}

void UTemplateStatsAttributesSet::OnRep_Damage(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateStatsAttributesSet , Damage , OldValue);
}
