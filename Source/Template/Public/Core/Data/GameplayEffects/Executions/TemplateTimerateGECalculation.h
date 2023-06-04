// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Core/Data/GameplayEffects/Executions/TemplateGECalculation.h"
#include "TemplateTimerateGECalculation.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATE_API UTemplateTimerateGECalculation : public UTemplateGECalculation
{
	GENERATED_BODY()
public:

	UTemplateTimerateGECalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams , FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
