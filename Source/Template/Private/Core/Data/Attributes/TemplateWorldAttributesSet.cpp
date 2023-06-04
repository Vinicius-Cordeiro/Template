// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Data/Attributes/TemplateWorldAttributesSet.h"

#include "Core/Characters/TemplateCharacter.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

void UTemplateWorldAttributesSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTemplateWorldAttributesSet , WorldTimecycleRate);
}

void UTemplateWorldAttributesSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
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


	// Set the timecycle rate attribute to update the world actions 
	if (Data.EvaluatedData.Attribute == GetWorldTimecycleRateAttribute()) {
		SetWorldTimecycleRate(FMath::Clamp(GetWorldTimecycleRate() , 0.000001f , 240000.f));
	}
}

void UTemplateWorldAttributesSet::PreAttributeChange(const FGameplayAttribute& Attribute , float& NewValue) {
	Super::PreAttributeChange(Attribute,NewValue);
}

void UTemplateWorldAttributesSet::OnRep_WorldTimecycleRate(const FGameplayAttributeData& OldValue) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTemplateWorldAttributesSet , WorldTimecycleRate , OldValue);
}
