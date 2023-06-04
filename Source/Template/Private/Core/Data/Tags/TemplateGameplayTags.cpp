// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Data/Tags/TemplateGameplayTags.h"

#include "GameplayTagsManager.h"
#include "Engine/EngineTypes.h"

FTemplateGameplayTags FTemplateGameplayTags::GameplayTags;

void FTemplateGameplayTags::InitializeNativeTags() { 
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(GameplayTagsManager);
	GameplayTagsManager.DoneAddingNativeTags();
}

//Registers all of the tags with the GameplayTags Manager
void FTemplateGameplayTags::AddAllTags(UGameplayTagsManager& Manager) {


	AddInputTags();
	AddStateTags();
	AddActionTags();
	AddTag(Tag_ClearWeather , "World.Weather.Clear" , " Clear Weather Tag");
	AddTag(Tag_WindyWeather , "World.Weather.Windy" , " Windy Weather Tag");
	AddTag(Tag_OvercastWeather , "World.Weather.Overcast" , " Overcast Weather Tag");
	AddTag(Tag_FoggyWeather , "World.Weather.Foggy" , " Foggy Weather Tag");
	AddTag(Tag_RainWeather , "World.Weather.Rain" , " Rain Weather Tag");
	AddTag(Tag_HeavyRainWeather , "World.Weather.HeavyRain" , " Heavy Rain Weather Tag");
	AddTag(Tag_StormWeather , "World.Weather.Storm" , " Storm Weather Tag");


}

//Helper function used by AddAllTags to register a single tag with the GameplayTags Manager
void FTemplateGameplayTags::AddTag(FGameplayTag& OutTag , const ANSICHAR* TagName , const ANSICHAR* TagComment) {
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName) , FString(TEXT("(Native) ")) + FString(TagComment));
}

void FTemplateGameplayTags::AddInputTags() {
	AddTag(InputTag_Move , "Input.Move" , "Move input.");
	AddTag(InputTag_Look_Mouse , "Input.Look.Mouse" , "Look (mouse) input.");
	AddTag(InputTag_Look_Stick , "Input.Look.Stick" , "Look (stick) input.");
	AddTag(InputTag_Jump , "Input.Jump" , "Jump input");
	AddTag(InputTag_Fire , "Input.Fire" , "Fire input.");
	AddTag(InputTag_ChangeShoulder , "Input.ChangeShoulder" , "ChangeShoulder input.");
	AddTag(InputTag_ADS , "Input.ADS" , "Tag for Aim Down Sights Input ");
	AddTag(InputTag_Crouch , "Input.Crouch" , "Crouch input.");
	AddTag(InputTag_Roll , "Input.Roll" , "Roll Input for dodge");
	AddTag(InputTag_Block , "Input.Block" , "block Input for avoid melee damage ");
	AddTag(InputTag_Punch , "Input.Punch" , "Punch Input.");
	AddTag(InputTag_Punch , "Input.Punch2" , "Second punch input type");
	AddTag(InputTag_Kick , "Input.Kick" , "Kick Input.");
	AddTag(InputTag_IncreaseSpeed , "Input.IncreaseSpeed" , "Increase Characters Speed");
	AddTag(InputTag_DecreaseSpeed , "Input.DecreaseSpeed" , "Decrease Characters Speed");
	AddTag(InputTag_Sprint , "Input.Sprint" , "Makes the character starts sprinting ");
	AddTag(InputTag_IncreaseCameraDistance , "Input.IncreaseCameraDistance" , "Increase Characters CameraDistance");
	AddTag(InputTag_DecreaseCameraDistance , "Input.DecreaseCameraDistance" , "Decrease Characters CameraDistance");
	AddTag(InputTag_OrbitCharacter , "Input.OrbitCharacter" , "Orbit Character Input ");
}

void FTemplateGameplayTags::AddStateTags() {
	AddTag(Tag_Ragdoll , "State.Ragdoll" , "Ragdolling Tag");
	AddTag(Tag_Falling , "State.Falling" , "Falling Tag");
	AddTag(Tag_Jumping , "State.Jumping" , "Jumping Tag");
	AddTag(Tag_Landing , "State.Landing" , "Landing Tag");
	AddTag(Tag_Swimming , "State.Swimming" , "Swimming Tag");
	AddTag(Tag_Sprinting , "State.Sprinting" , "Sprinting Tag");
	AddTag(Tag_Moving , "State.Moving" , "Moving Tag");
	AddTag(Tag_Crouching , "State.Crouching" , "Crouching Tag");
	AddTag(Tag_Standing , "State.Standing" , "Standing Tag");
	AddTag(Tag_Rolling , "State.Rolling" , "Rolling Tag");
}

void FTemplateGameplayTags::AddActionTags() {
	AddTag(Tag_MirrorAnimations , "Action.Mirror" , "Mirror animation Tag ");

	AddTag(Tag_RollForward , "Action.Animations.RollForward" , "Action Roll Forward Tag");
	AddTag(Tag_RollForwardRight , "Action.Animations.RollForwardRight" , "Action Roll Forward Right Tag");
	AddTag(Tag_RollForwardLeft , "Action.Animations.RollForwardLeft" , "Action Roll Forward Left Tag");
	AddTag(Tag_RollBackward , "Action.Animations.RollBackward" , "Action Roll Backward Tag");
	AddTag(Tag_RollBackwardRight , "Action.Animations.RollBackwardRight" , "Action Roll Backward Right Tag");
	AddTag(Tag_RollBackwardLeft , "Action.Animations.RollBackwardLeft" , "Action Roll Backward Left Tag");
	AddTag(Tag_RollRight , "Action.Animations.RollRight" , "Action Roll Right Tag");
	AddTag(Tag_RollLeft , "Action.Animations.RollLeft" , "Action Roll Left Tag");
}

void FTemplateGameplayTags::AddPhysicalMaterialsTags() {
	AddTag(Tag_DEFAULTPHYSMATERIAL , "Physics.PhysicalMaterial.DEFAULT" , "DEFAULT PHYSICAL MATERIAL TAG");
	AddTag(Tag_ACID , "Physics.PhysicalMaterial.ACID" , "ACID PHYSICAL MATERIAL Tag");
	AddTag(Tag_ASPHALT , "Physics.PhysicalMaterial.ASPHALT" , "ASPHALT PHYSICAL MATERIAL Tag");
	AddTag(Tag_BARK , "Physics.PhysicalMaterial.BARK" , "BARK PHYSICAL MATERIAL TAG");
	AddTag(Tag_BLOOD , "Physics.PhysicalMaterial.BLOOD " , "BLOOD PHYSICAL MATERIAL TAG");
	AddTag(Tag_BLOODCRITICAL , "Physics.PhysicalMaterial.BLOODCRITICAL " , "BLOODCRITICAL PHYSICAL MATERIAL TAG");
	AddTag(Tag_BONES , "Physics.PhysicalMaterial.BONES " , "BONES PHYSICAL MATERIAL TAG");
	AddTag(Tag_BRICK , "Physics.PhysicalMaterial.BRICK " , "BRICK PHYSICAL MATERIAL TAG");
	AddTag(Tag_BROKENGLASS , "Physics.PhysicalMaterial.BROKENGLASS " , "BROKENGLASS PHYSICAL MATERIAL TAG");
	AddTag(Tag_CARDBOARD , "Physics.PhysicalMaterial.CARDBOARD " , "CARDBOARD PHYSICAL MATERIAL TAG");
	AddTag(Tag_CARPET , "Physics.PhysicalMaterial.CARPET " , "CARPET PHYSICAL MATERIAL TAG");
	AddTag(Tag_CARBONFIBER , "Physics.PhysicalMaterial.CARBONFIBER " , "CARBONFIBER PHYSICAL MATERIAL TAG");
	AddTag(Tag_CERAMIC , "Physics.PhysicalMaterial.CERAMIC " , "CERAMIC PHYSICAL MATERIAL TAG");
	AddTag(Tag_CONCRETE , "Physics.PhysicalMaterial.CONCRETE " , "CONCRETE PHYSICAL MATERIAL TAG");
	AddTag(Tag_CORRUGATEDSTEEL , "Physics.PhysicalMaterial.CORRUGATEDSTEEL " , "CORRUGATEDSTEEL PHYSICAL MATERIAL TAG");
	AddTag(Tag_DIRT , "Physics.PhysicalMaterial.DIRT " , "DIRT PHYSICAL MATERIAL TAG");
	AddTag(Tag_DIRT2 , "Physics.PhysicalMaterial.DIRT2 " , "DIRT2 PHYSICAL MATERIAL TAG");
	AddTag(Tag_ELECTRICAL , "Physics.PhysicalMaterial.ELECTRICAL " , "ELECTRICAL PHYSICAL MATERIAL TAG");
	AddTag(Tag_FABRIC , "Physics.PhysicalMaterial.FABRIC " , "FABRIC PHYSICAL MATERIAL TAG");
	AddTag(Tag_FENCE , "Physics.PhysicalMaterial.FENCE " , "FENCE PHYSICAL MATERIAL TAG");
	AddTag(Tag_GLASS , "Physics.PhysicalMaterial.GLASS " , "GLASS PHYSICAL MATERIAL TAG");
	AddTag(Tag_GRASS , "Physics.PhysicalMaterial.GRASS " , "GRASS PHYSICAL MATERIAL TAG");
	AddTag(Tag_GRAVEL , "Physics.PhysicalMaterial.GRAVEL " , "GRAVEL PHYSICAL MATERIAL TAG");
	AddTag(Tag_ICE , "Physics.PhysicalMaterial.ICE " , "ICE PHYSICAL MATERIAL TAG");
	AddTag(Tag_LEATHER , "Physics.PhysicalMaterial.LEATHER " , "LEATHER PHYSICAL MATERIAL TAG");
	AddTag(Tag_LEAVES , "Physics.PhysicalMaterial.LEAVES " , "LEAVES PHYSICAL MATERIAL TAG");
	AddTag(Tag_LIMESTONE , "Physics.PhysicalMaterial.LIMESTONE " , "LIMESTONE PHYSICAL MATERIAL TAG");
	AddTag(Tag_METAL , "Physics.PhysicalMaterial.METAL " , "METAL PHYSICAL MATERIAL TAG");
	AddTag(Tag_METALDULL , "Physics.PhysicalMaterial.METALDULL " , "METALDULL PHYSICAL MATERIAL TAG");
	AddTag(Tag_MUD , "Physics.PhysicalMaterial.MUD " , "MUD PHYSICAL MATERIAL TAG");
	AddTag(Tag_OIL , "Physics.PhysicalMaterial.OIL " , "OIL PHYSICAL MATERIAL TAG");
	AddTag(Tag_PAPER , "Physics.PhysicalMaterial.PAPER " , "PAPER PHYSICAL MATERIAL TAG");
	AddTag(Tag_PLASTIC , "Physics.PhysicalMaterial.PLASTIC " , "PLASTIC PHYSICAL MATERIAL TAG");
	AddTag(Tag_POLYSTYRENE , "Physics.PhysicalMaterial.POLYSTYRENE " , "POLYSTYRENE PHYSICAL MATERIAL TAG");
	AddTag(Tag_ROCK , "Physics.PhysicalMaterial.ROCK " , "ROCK PHYSICAL MATERIAL TAG");
	AddTag(Tag_ROOFTILES , "Physics.PhysicalMaterial.ROOFTILES " , "ROOFTILES PHYSICAL MATERIAL TAG");
	AddTag(Tag_RUBBER , "Physics.PhysicalMaterial.RUBBER " , "RUBBER PHYSICAL MATERIAL TAG");
	AddTag(Tag_SAND , "Physics.PhysicalMaterial.SAND " , "SAND PHYSICAL MATERIAL TAG");
	AddTag(Tag_SOIL , "Physics.PhysicalMaterial.SOIL " , "SOIL PHYSICAL MATERIAL TAG");
	AddTag(Tag_TILES , "Physics.PhysicalMaterial.TILES " , "TILES PHYSICAL MATERIAL TAG");
	AddTag(Tag_TRASH , "Physics.PhysicalMaterial.TRASH " , "TRASH PHYSICAL MATERIAL TAG");
	AddTag(Tag_VEST , "Physics.PhysicalMaterial.VEST " , "VEST PHYSICAL MATERIAL TAG");
	AddTag(Tag_VEGETATION , "Physics.PhysicalMaterial.VEGETATION " , "VEGETATION PHYSICAL MATERIAL TAG");
	AddTag(Tag_WATER , "Physics.PhysicalMaterial.WATER " , "WATER PHYSICAL MATERIAL TAG");
	AddTag(Tag_WATERHEAVY , "Physics.PhysicalMaterial.WATERHEAVY " , "WATERHEAVY PHYSICAL MATERIAL TAG");
	AddTag(Tag_WOOD , "Physics.PhysicalMaterial.WOOD " , "WOOD PHYSICAL MATERIAL TAG");
}
