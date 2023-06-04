// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TemplateItem.generated.h"

UCLASS(ClassGroup = (Custom) , meta = (BlueprintSpawnableComponent))
class TEMPLATE_API ATemplateItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATemplateItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
