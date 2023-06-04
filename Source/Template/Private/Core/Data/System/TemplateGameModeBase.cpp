// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#include "Core/Data/System/TemplateGameModeBase.h"

#include "Core/Data/System/TemplateGameState.h"
#include "Core/Characters/TemplateCharacter.h"
#include "Core/Characters/Player/TemplatePlayerController.h"
#include "Core/UI/Main/TemplateHUD.h"

ATemplateGameModeBase::ATemplateGameModeBase(const FObjectInitializer& ObjectInitializer) {

	static ConstructorHelpers::FClassFinder<ATemplateGameState> GameStateClassRef = TEXT("/Game/Template/Core/Data/System/GS_Template.GS_Template_C");
	if (GameStateClassRef.Succeeded()) GameStateClass = GameStateClassRef.Class;

	static ConstructorHelpers::FClassFinder<ATemplateCharacter> PlayerCharacterClass = TEXT("/Game/Template/Core/Characters/Player/BP_PlayerCharacter.BP_PlayerCharacter_C");
	if (PlayerCharacterClass.Succeeded()) DefaultPawnClass = PlayerCharacterClass.Class;

	static ConstructorHelpers::FClassFinder<ATemplatePlayerController> PlayerControllerClassRef = TEXT("/Game/Template/Core/Characters/Player/BP_PlayerController.BP_PlayerController_C");
	if (PlayerControllerClassRef.Succeeded()) PlayerControllerClass = PlayerControllerClassRef.Class;

	static ConstructorHelpers::FClassFinder<ATemplateHUD> PlayerHUD = TEXT("/Game/Template/Core/UI/HUD_Template.HUD_Template_C");
	if (PlayerHUD.Succeeded()) HUDClass = PlayerHUD.Class;

}
