// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags/Classes/GameplayTagContainer.h"

class UGameplayTagsManager;


struct FTemplateGameplayTags {
public:
	static const FTemplateGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	/** Input action pressing tag mapping */
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;
	FGameplayTag InputTag_Look_Stick;
	FGameplayTag InputTag_Jump;
	FGameplayTag InputTag_Fire;
	FGameplayTag InputTag_ChangeShoulder;
	FGameplayTag InputTag_ADS;
	FGameplayTag InputTag_Crouch;
	FGameplayTag InputTag_Roll;
	FGameplayTag InputTag_Block;
	FGameplayTag InputTag_Punch;
	FGameplayTag InputTag_Punch2;
	FGameplayTag InputTag_Kick;
	FGameplayTag InputTag_Sprint;
	FGameplayTag InputTag_IncreaseSpeed;
	FGameplayTag InputTag_DecreaseSpeed;
	FGameplayTag InputTag_IncreaseCameraDistance;
	FGameplayTag InputTag_DecreaseCameraDistance;
	FGameplayTag InputTag_OrbitCharacter;
	/** Chord  key presses */
	FGameplayTag InputTag_AltPressing;
	FGameplayTag InputTag_ShiftPressing;
	FGameplayTag InputTag_CtrlPressing;

	/** Default Native Tags : states, movements, etc..*/
	FGameplayTag Tag_Ragdoll;
	FGameplayTag Tag_Falling;
	FGameplayTag Tag_Jumping;
	FGameplayTag Tag_Landing;
	FGameplayTag Tag_Swimming;
	FGameplayTag Tag_Sprinting;
	FGameplayTag Tag_Moving;
	FGameplayTag Tag_Crouching;
	FGameplayTag Tag_Standing;
	FGameplayTag Tag_Rolling;

	FGameplayTag Tag_MirrorAnimations;

	FGameplayTag Tag_RollForward;
	FGameplayTag Tag_RollForwardRight;
	FGameplayTag Tag_RollForwardLeft;
	FGameplayTag Tag_RollBackward;
	FGameplayTag Tag_RollBackwardRight;
	FGameplayTag Tag_RollBackwardLeft;
	FGameplayTag Tag_RollRight;
	FGameplayTag Tag_RollLeft;

	FGameplayTag Tag_ClearWeather;
	FGameplayTag Tag_WindyWeather;
	FGameplayTag Tag_OvercastWeather;
	FGameplayTag Tag_FoggyWeather;
	FGameplayTag Tag_RainWeather;
	FGameplayTag Tag_HeavyRainWeather;
	FGameplayTag Tag_StormWeather;

	/**
	*	Definitions of the surface type materials used to decide what particle to play when a bullet hit,
	*	or the sound of the footstep should play based on the surface
	*/
	FGameplayTag Tag_DEFAULTPHYSMATERIAL;
	FGameplayTag Tag_ACID;
	FGameplayTag Tag_ASPHALT;
	FGameplayTag Tag_BARK;
	FGameplayTag Tag_BLOOD;
	FGameplayTag Tag_BLOODCRITICAL;
	FGameplayTag Tag_BONES;
	FGameplayTag Tag_BRICK;
	FGameplayTag Tag_BROKENGLASS;
	FGameplayTag Tag_CARDBOARD;
	FGameplayTag Tag_CARPET;
	FGameplayTag Tag_CARBONFIBER;
	FGameplayTag Tag_CERAMIC;
	FGameplayTag Tag_CONCRETE;
	FGameplayTag Tag_CORRUGATEDSTEEL;
	FGameplayTag Tag_DIRT;
	FGameplayTag Tag_DIRT2;
	FGameplayTag Tag_ELECTRICAL;
	FGameplayTag Tag_FABRIC;
	FGameplayTag Tag_FENCE;
	FGameplayTag Tag_GLASS;
	FGameplayTag Tag_GRASS;
	FGameplayTag Tag_GRAVEL;
	FGameplayTag Tag_ICE;
	FGameplayTag Tag_LEATHER;
	FGameplayTag Tag_LEAVES;
	FGameplayTag Tag_LIMESTONE;
	FGameplayTag Tag_METAL;
	FGameplayTag Tag_METALDULL;
	FGameplayTag Tag_MUD;
	FGameplayTag Tag_OIL;
	FGameplayTag Tag_PAPER;
	FGameplayTag Tag_PLASTIC;
	FGameplayTag Tag_POLYSTYRENE;
	FGameplayTag Tag_ROCK;
	FGameplayTag Tag_ROOFTILES;
	FGameplayTag Tag_RUBBER;
	FGameplayTag Tag_SAND;
	FGameplayTag Tag_SOIL;
	FGameplayTag Tag_TILES;
	FGameplayTag Tag_TRASH;
	FGameplayTag Tag_VEST;
	FGameplayTag Tag_VEGETATION;
	FGameplayTag Tag_WATER;
	FGameplayTag Tag_WATERHEAVY;
	FGameplayTag Tag_WOOD;


protected:

	//Registers all of the tags with the GameplayTags Manager
	void AddAllTags(UGameplayTagsManager& Manager);

	//Helper function used by AddAllTags to register a single tag with the GameplayTags Manager
	void AddTag(FGameplayTag& OutTag , const ANSICHAR* TagName , const ANSICHAR* TagComment);

	void AddInputTags();

	void AddStateTags();

	void AddActionTags();

	void AddPhysicalMaterialsTags();

private:

	static FTemplateGameplayTags GameplayTags;

};