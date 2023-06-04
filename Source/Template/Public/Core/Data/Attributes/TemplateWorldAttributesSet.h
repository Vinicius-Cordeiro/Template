// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Core/Data/Attributes/TemplateAttributesSet.h"
#include "TemplateWorldAttributesSet.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATE_API UTemplateWorldAttributesSet : public UTemplateAttributesSet
{
	GENERATED_BODY()
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute , float& NewValue) override;

	/** World Timecycle rate used to control the speed of attribute consumption , decay on the world , etc.. */
	UPROPERTY(BlueprintReadOnly , Category = Attributes , ReplicatedUsing = OnRep_WorldTimecycleRate)
	FGameplayAttributeData WorldTimecycleRate;
	ATTRIBUTE_ACCESSORS(UTemplateWorldAttributesSet , WorldTimecycleRate);

	UFUNCTION()
	void OnRep_WorldTimecycleRate(const FGameplayAttributeData& OldValue);
};
