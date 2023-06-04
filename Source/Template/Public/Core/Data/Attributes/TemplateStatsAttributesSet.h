// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Core/Data/Attributes/TemplateAttributesSet.h"
#include "TemplateStatsAttributesSet.generated.h"

class ATemplateCharacter;

/**
 * 
 */
UCLASS()
class TEMPLATE_API UTemplateStatsAttributesSet : public UTemplateAttributesSet
{
	GENERATED_BODY()
public:
	UTemplateStatsAttributesSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute , float& NewValue) override;


public:


	/** Current health */
	UPROPERTY(BlueprintReadOnly , Category = "Attributes" , ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTemplateStatsAttributesSet , Health);

	/** Max Health */
	UPROPERTY(BlueprintReadOnly , Category = "Attributes" , ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTemplateStatsAttributesSet , MaxHealth);

	/** Current Stamina */
	UPROPERTY(BlueprintReadOnly , Category = "Attributes" , ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UTemplateStatsAttributesSet , Stamina);

	/** Max Stamina */
	UPROPERTY(BlueprintReadOnly , Category = "Attributes" , ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UTemplateStatsAttributesSet , MaxStamina);


	/** Current Nutrition */
	UPROPERTY(BlueprintReadOnly , Category = "Attributes" , ReplicatedUsing = OnRep_Nutrition)
	FGameplayAttributeData Nutrition;
	ATTRIBUTE_ACCESSORS(UTemplateStatsAttributesSet , Nutrition);

	/** Max Nutrition */
	UPROPERTY(BlueprintReadOnly , Category = "Attributes" , ReplicatedUsing = OnRep_MaxNutrition)
	FGameplayAttributeData MaxNutrition;
	ATTRIBUTE_ACCESSORS(UTemplateStatsAttributesSet , MaxNutrition);

	/** Current Hydration */
	UPROPERTY(BlueprintReadOnly , Category = "Attributes" , ReplicatedUsing = OnRep_Hydration)
	FGameplayAttributeData Hydration;
	ATTRIBUTE_ACCESSORS(UTemplateStatsAttributesSet , Hydration);

	/** Max Hydration */
	UPROPERTY(BlueprintReadOnly , Category = "Attributes" , ReplicatedUsing = OnRep_MaxHydration)
	FGameplayAttributeData MaxHydration;
	ATTRIBUTE_ACCESSORS(UTemplateStatsAttributesSet , MaxHydration);

	/** Current Fatigue */
	UPROPERTY(BlueprintReadOnly , Category = "Attributes" , ReplicatedUsing = OnRep_Fatigue)
	FGameplayAttributeData Fatigue;
	ATTRIBUTE_ACCESSORS(UTemplateStatsAttributesSet , Fatigue);

	/** Max Fatigue */
	UPROPERTY(BlueprintReadOnly , Category = "Attributes" , ReplicatedUsing = OnRep_MaxFatigue)
	FGameplayAttributeData MaxFatigue;
	ATTRIBUTE_ACCESSORS(UTemplateStatsAttributesSet , MaxFatigue);

	UPROPERTY(BlueprintReadOnly , Category = "Attributes" , ReplicatedUsing = OnRep_Damage)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UTemplateStatsAttributesSet , Damage);


	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Nutrition(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxNutrition(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Hydration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxHydration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Fatigue(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxFatigue(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Damage(const FGameplayAttributeData& OldValue);
friend ATemplateCharacter;
};
