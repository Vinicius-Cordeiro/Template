// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Data/Input/TemplateInputConfig.h"

// Engine Includes 
#include "GameplayTagContainer.h"
#include "EnhancedInput/Public/InputAction.h"

const UInputAction* UTemplateInputConfig::FindInputActionForTag(const FGameplayTag& InputTag) const {
	for (const FTaggedInputAction& TaggedInputAction : TaggedInputActions) {
		if (TaggedInputAction.InputAction && TaggedInputAction.Tag == InputTag) {
			return TaggedInputAction.InputAction;
		}
	}
	return nullptr;
}
