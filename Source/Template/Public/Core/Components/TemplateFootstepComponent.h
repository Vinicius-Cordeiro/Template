// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TemplateFootstepComponent.generated.h"

/**
*	Footstep component responsible for storing data for footsteps sounds, niagara particles , footprint decals
*	Using data asset as a base mapping point, which will be changed base on the actor that uses this component.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEMPLATE_API UTemplateFootstepComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTemplateFootstepComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
