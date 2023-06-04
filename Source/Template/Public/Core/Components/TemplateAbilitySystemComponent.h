// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TemplateAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATE_API UTemplateAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	virtual void OnTagUpdated(const FGameplayTag& Tag , bool TagExists) override;

	/** Return a mutable pointer to the ActiveGameplayEffect from the supplied handle. */
	FActiveGameplayEffect* GetActiveGameplayEffect_Mutable(FActiveGameplayEffectHandle Handle);

	/** Returns all active gameplay effect handles */
	TArray<FActiveGameplayEffectHandle> GetAllActiveEffectHandles() const;

	/** Marks the ActiveGameplayEffect as dirty for replication purposes */
	void MarkActiveGameplayEffectDirty(FActiveGameplayEffect* ActiveGE);

	/** Checks the active effect duration and runs any logic, checks to see if the GE is expired and removes it. */
	void CheckActiveEffectDuration(const FActiveGameplayEffectHandle& Handle);

};
