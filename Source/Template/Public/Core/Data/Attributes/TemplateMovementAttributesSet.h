// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Core/Data/Attributes/TemplateAttributesSet.h"
#include "TemplateMovementAttributesSet.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATE_API UTemplateMovementAttributesSet : public UTemplateAttributesSet
{
	GENERATED_BODY()
public:
	
	UTemplateMovementAttributesSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute , float& NewValue) override;


public:


	/** Current Velocity */
	UPROPERTY(BlueprintReadOnly , Category = Attributes , ReplicatedUsing = OnRep_CurrentVelocity)
	FGameplayAttributeData CurrentVelocity;
	ATTRIBUTE_ACCESSORS(UTemplateMovementAttributesSet , CurrentVelocity);

	/** Min Velocity */
	UPROPERTY(BlueprintReadOnly , Category = Attributes , ReplicatedUsing = OnRep_MinVelocity)
	FGameplayAttributeData MinVelocity;
	ATTRIBUTE_ACCESSORS(UTemplateMovementAttributesSet , MinVelocity);

	/** Max Velocity */
	UPROPERTY(BlueprintReadOnly , Category = Attributes , ReplicatedUsing = OnRep_MaxVelocity)
	FGameplayAttributeData MaxVelocity;
	ATTRIBUTE_ACCESSORS(UTemplateMovementAttributesSet , MaxVelocity);

	UPROPERTY(BlueprintReadOnly,Category = Attributes , ReplicatedUsing = OnRep_DamageHeightThreshold)
	FGameplayAttributeData DamageHeightThreshold;
	ATTRIBUTE_ACCESSORS(UTemplateMovementAttributesSet , DamageHeightThreshold);

	UPROPERTY(BlueprintReadOnly , Category = Attributes , ReplicatedUsing = OnRep_ZVelocity)
	FGameplayAttributeData ZVelocity;
	ATTRIBUTE_ACCESSORS(UTemplateMovementAttributesSet , ZVelocity);

	
	UFUNCTION()
	virtual void OnRep_CurrentVelocity(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MinVelocity(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxVelocity(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_DamageHeightThreshold(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_ZVelocity(const FGameplayAttributeData& OldValue);

};
