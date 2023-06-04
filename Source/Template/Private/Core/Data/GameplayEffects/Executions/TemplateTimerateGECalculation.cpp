// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Data/GameplayEffects/Executions/TemplateTimerateGECalculation.h"

#include "Core/Data/Attributes/TemplateWorldAttributesSet.h"
#include "Core/Components/TemplateAbilitySystemComponent.h"


struct FTimecycleRateStatics
{
	//Target
	DECLARE_ATTRIBUTE_CAPTUREDEF(WorldTimecycleRate);

	FTimecycleRateStatics() {
		// Capture the Timecycle Rate Multiplier value which will modify the 'cost/recover' of the attribute / gameplay effect
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTemplateWorldAttributesSet , WorldTimecycleRate , Source , true);
	}
};

static FTimecycleRateStatics& TimecycleRateStatics() {
	static FTimecycleRateStatics Statics;
	return Statics;
}

UTemplateTimerateGECalculation::UTemplateTimerateGECalculation() {
	RelevantAttributesToCapture.Add(TimecycleRateStatics().WorldTimecycleRateDef);
}

void UTemplateTimerateGECalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams , FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
	Super::Execute_Implementation(ExecutionParams , OutExecutionOutput);

	const UTemplateAbilitySystemComponent* TargetAbilitySystemComponent { Cast<UTemplateAbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent()) };

	AActor* TargetActor { TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor_Direct() : nullptr };

	const FGameplayEffectSpec& Spec { ExecutionParams.GetOwningSpec() };

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags { Spec.CapturedSourceTags.GetAggregatedTags() };
	const FGameplayTagContainer* TargetTags { Spec.CapturedTargetTags.GetAggregatedTags() };

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float WorldTimecycleRate { 0.f };
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TimecycleRateStatics().WorldTimecycleRateDef , EvaluationParameters , WorldTimecycleRate);

	if (!Spec.Def->Modifiers.IsValidIndex(0) || !Spec.Def->Modifiers[0].Attribute.IsValid()) {
		return;
	}

	FGameplayModifierEvaluatedData Data;
	Data.Attribute = Spec.Def->Modifiers[0].Attribute;
	Data.Magnitude = FMath::Abs(WorldTimecycleRate);
	Data.ModifierOp = EGameplayModOp::Division;

	OutExecutionOutput.AddOutputModifier(Data);
}
