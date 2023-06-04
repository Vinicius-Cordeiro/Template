// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TemplateCrosshairWidget.generated.h"


class UTemplateCrosshairDynamicWidget;

/**
 * 
 */
UCLASS()
class TEMPLATE_API UTemplateCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	/** Default Dynamic Crosshair Widget */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = UserInterface , meta = (BindWidget))
	TObjectPtr<UTemplateCrosshairDynamicWidget> DynamicCrosshair { nullptr };
};
