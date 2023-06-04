// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TemplateFootstepDataAsset.generated.h"

UCLASS()
class TEMPLATE_API UTemplateFootstepDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:

	/** Decal Material to be spawned on the world based on the surface the character is upon */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Footsteps")
	TObjectPtr<class UMaterialInterface>  DecalMaterial;

	/** Niagara particle FX to be spawned on the world based on the surface the character is upon */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Footsteps")
	TObjectPtr<class UNiagaraSystem> NiagaraParticleFX;

	/** Sound CUE / Base which will be played whenever a valid collision is found */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Footsteps")
	TObjectPtr<class USoundBase> FootstepSoundSFX;

	/** How loud the footstep effect will be played, this is clamping the final volume, the volume is decided physically based on the speed, movement mode/state */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Footsteps")
	FVector2D FootstepVolume { FVector2D(0.9f , 1.25f) };

	/** The sound effect pitch when playing the footstep SFX */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Footsteps")
	FVector2D FootstepPitch { FVector2D(.9f , 1.1f) };
};
