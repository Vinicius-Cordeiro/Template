// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TemplateHUDWidget.generated.h"

class UTemplateCrosshairWidget;

/**
 *
 */
UCLASS()
class TEMPLATE_API UTemplateHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	/** Default  Crosshair Widget */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = UserInterface,meta=(BindWidget))
	TObjectPtr<UTemplateCrosshairWidget> Crosshair { nullptr };
};
