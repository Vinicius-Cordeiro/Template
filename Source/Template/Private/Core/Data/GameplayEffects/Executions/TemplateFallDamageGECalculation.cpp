// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Data/GameplayEffects/Executions/TemplateFallDamageGECalculation.h"

#include "Core/Data/Attributes/TemplateMovementAttributesSet.h"
#include "Core/Data/Attributes/TemplateStatsAttributesSet.h"
#include "Core/Components/TemplateAbilitySystemComponent.h"

struct FDamageStaticsStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageHeightThreshold);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ZVelocity);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);

	FDamageStaticsStruct() {
		// Capture the height damage threshold which is set on ragdoll structure , so the damage will be damped 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTemplateMovementAttributesSet , DamageHeightThreshold , Source , false);

		// Also capture the source's raw ZVelocity, which is normally passed in directly via the execution 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTemplateMovementAttributesSet , ZVelocity , Source , true);

		// Capture the damage that will be changed on the attribute set raw damage
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTemplateStatsAttributesSet , Damage , Source , true);

		// Capture the damage that will be changed on the attribute set raw damage
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTemplateStatsAttributesSet , Health , Source , true);

		// Capture the damage that will be changed on the attribute set raw damage
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTemplateStatsAttributesSet , MaxHealth , Source , true);
	}
};

static const FDamageStaticsStruct& DamageStatics() {
	static FDamageStaticsStruct DmgStatics;
	return DmgStatics;
}

UTemplateFallDamageGECalculation::UTemplateFallDamageGECalculation() {
	RelevantAttributesToCapture.Add(DamageStatics().DamageHeightThresholdDef);
	RelevantAttributesToCapture.Add(DamageStatics().ZVelocityDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().MaxHealthDef);
}

void UTemplateFallDamageGECalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams , FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
	Super::Execute_Implementation(ExecutionParams , OutExecutionOutput);

	const UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	const UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// --------------------------------------
	//	Damage Done = Damage  / Height Damage Threshold Height 
	//	If Height Damage Threshold is 0, it is treated as 1.0
	// --------------------------------------

	float HeightDamageThreshold = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageHeightThresholdDef , EvaluationParameters , HeightDamageThreshold);
	if (HeightDamageThreshold == 0.0f) {
		HeightDamageThreshold = 1.0f;
	}

	float ZVelocityRawDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ZVelocityDef , EvaluationParameters , ZVelocityRawDamage);

	float Health = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HealthDef , EvaluationParameters , Health);

	float MaxHealth = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MaxHealthDef , EvaluationParameters , MaxHealth);

	// Constraint damage ratio
	const float NewDamageRatio { FMath::Abs(FMath::Abs(ZVelocityRawDamage) / FMath::Abs(HeightDamageThreshold)) * MaxHealth };

	GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Orange , FString("Calculated Damage : ") + FString::SanitizeFloat(NewDamageRatio) + " !");

	if (FMath::Abs(NewDamageRatio) > 0.f) {
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty , EGameplayModOp::Override , FMath::Abs(NewDamageRatio)));

	} else {
		GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Emerald , FString("Not a Valid Damage! ") + FString::SanitizeFloat(NewDamageRatio) + " !");
	}
}