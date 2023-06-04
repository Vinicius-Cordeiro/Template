// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TemplateHUD.generated.h"

class UTemplateHUDWidget;

/**
 * 
 */
UCLASS()
class TEMPLATE_API ATemplateHUD : public AHUD
{
	GENERATED_BODY()
public:

	/** Stored pointer for the hud widget */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = HUD)
	TObjectPtr<UTemplateHUDWidget> HUDWidget;


	/** Sub class of the hud widget , to the creation and initialization of the hud */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = HUD)
	TSubclassOf<UTemplateHUDWidget> HUDWidgetClass;

public:
	ATemplateHUD();

	virtual void BeginPlay() override;

	/** hides or shows HUD */
	virtual void ShowHUD() override;

	/** The Main Draw loop for the hud.  Gets called before any messaging.  Should be subclassed */
	virtual void DrawHUD() override;

};
