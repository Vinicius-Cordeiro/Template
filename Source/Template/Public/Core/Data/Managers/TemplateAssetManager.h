// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TemplateAssetManager.generated.h"

/**
 *
 */
UCLASS()
class TEMPLATE_API UTemplateAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	UTemplateAssetManager();

	static UTemplateAssetManager& Get();


protected:

	virtual void StartInitialLoading() override;

};
