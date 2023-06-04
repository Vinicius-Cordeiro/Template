// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Data/Managers/TemplateAssetManager.h"

#include "Core/Data/Tags/TemplateGameplayTags.h"


UTemplateAssetManager::UTemplateAssetManager() {
}

UTemplateAssetManager& UTemplateAssetManager::Get() {
	check(GEngine);

	UTemplateAssetManager* TemplateAssetManager = Cast<UTemplateAssetManager>(GEngine->AssetManager);

	return *TemplateAssetManager;
}

void UTemplateAssetManager::StartInitialLoading() {
	FTemplateGameplayTags::InitializeNativeTags();
}
