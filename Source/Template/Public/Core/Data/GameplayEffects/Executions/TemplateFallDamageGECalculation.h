// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Core/Data/GameplayEffects/Executions/TemplateGECalculation.h"
#include "TemplateFallDamageGECalculation.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATE_API UTemplateFallDamageGECalculation : public UTemplateGECalculation
{
	GENERATED_BODY()
public:

	UTemplateFallDamageGECalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams , OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
