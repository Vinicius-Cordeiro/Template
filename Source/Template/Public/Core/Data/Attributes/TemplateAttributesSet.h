// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "TemplateAttributesSet.generated.h"


// Macros from attribute set 
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 *	Base Attribute Set used to create attribute sets, with a default inherited behavior  
 */
UCLASS()
class TEMPLATE_API UTemplateAttributesSet : public UAttributeSet
{
	GENERATED_BODY()
public:

	UTemplateAttributesSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute , float& NewValue) override;

	virtual void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute , const FGameplayAttributeData& MaxAttribute , float NewMaxValue , const FGameplayAttribute& AffectedAttributeProperty) const;

};
