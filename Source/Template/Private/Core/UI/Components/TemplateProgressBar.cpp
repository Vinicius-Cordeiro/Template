// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/UI/Components/TemplateProgressBar.h"

bool UTemplateProgressBar::Initialize() {
	const bool bWasWidgetInitialized { Super::Initialize() };
	return false;
}

void UTemplateProgressBar::NativeConstruct() {
	Super::NativeConstruct();
}

void UTemplateProgressBar::NativeTick(const FGeometry& MyGeometry , float InDeltaTime) {
	Super::NativeTick(MyGeometry , InDeltaTime);
}
