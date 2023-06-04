// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/UI/Main/TemplateHUD.h"

#include "Core/UI/Main/TemplateHUDWidget.h"

ATemplateHUD::ATemplateHUD() {
}

void ATemplateHUD::BeginPlay() {
	Super::BeginPlay();

	if(HUDWidgetClass){
		// Does a check if we have a valid widget class to create the hud widget
		HUDWidget = CreateWidget<UTemplateHUDWidget>(GetOwningPlayerController() , HUDWidgetClass);
		if (!HUDWidget) { if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,"Could not Create hud Widget!!!"); return; }

		HUDWidget->AddToViewport(999);
	}
}

void ATemplateHUD::ShowHUD() {
	Super::ShowHUD();

	// Change visibility of the hud widget based on the HUD show hud variable 
	if(HUDWidget){
		HUDWidget->SetVisibility(bShowHUD ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void ATemplateHUD::DrawHUD() {
	Super::DrawHUD();
}
