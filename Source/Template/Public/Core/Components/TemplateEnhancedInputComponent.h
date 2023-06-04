// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Core/Data/Input/TemplateInputConfig.h"
#include "GameplayTagContainer.h"
#include "TemplateEnhancedInputComponent.generated.h"

/**
 *
 */
UCLASS()
class TEMPLATE_API UTemplateEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	template<class UserClass , typename FuncType>
	void BindActionByTag(const UTemplateInputConfig* InputConfig , const FGameplayTag& InputTag , ETriggerEvent TriggerEvent , UserClass* Object , FuncType Func);
};

template<class UserClass , typename FuncType>
void UTemplateEnhancedInputComponent::BindActionByTag(const UTemplateInputConfig* InputConfig , const FGameplayTag& InputTag , ETriggerEvent TriggerEvent , UserClass* Object , FuncType Func) {
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindInputActionForTag(InputTag)) {
		BindAction(IA , TriggerEvent , Object , Func);
	}
}

