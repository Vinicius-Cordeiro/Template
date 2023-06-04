// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TemplateInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLATE_API UTemplateInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTemplateInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
