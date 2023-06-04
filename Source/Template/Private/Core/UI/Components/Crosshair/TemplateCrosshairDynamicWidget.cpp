// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/UI/Components/Crosshair/TemplateCrosshairDynamicWidget.h"


bool UTemplateCrosshairDynamicWidget::Initialize() {
	const bool bWasWidgetInitialized { Super::Initialize() };
	return false;
}

void UTemplateCrosshairDynamicWidget::NativeConstruct() {
	Super::NativeConstruct();

}

void UTemplateCrosshairDynamicWidget::NativeTick(const FGeometry& MyGeometry , float InDeltaTime) {
	Super::NativeTick(MyGeometry , InDeltaTime);

}
