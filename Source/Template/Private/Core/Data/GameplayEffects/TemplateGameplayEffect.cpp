// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Data/GameplayEffects/TemplateGameplayEffect.h"

#include "Core/Data/GameplayEffects/Executions/TemplateTimerateGECalculation.h"

UTemplateGameplayEffect::UTemplateGameplayEffect() {
	FGameplayEffectExecutionDefinition CustomCalculations;
	CustomCalculations.CalculationClass = UTemplateTimerateGECalculation::StaticClass();
	FGameplayEffectExecutionScopedModifierInfo ScopedExecutionModifierInfo;
	ScopedExecutionModifierInfo.ModifierOp = EGameplayModOp::Division;
	ScopedExecutionModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(1.f);
	CustomCalculations.CalculationModifiers.Add(ScopedExecutionModifierInfo);
	Executions.Add(CustomCalculations);
}
