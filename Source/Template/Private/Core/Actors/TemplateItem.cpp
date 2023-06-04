// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#include "Core/Actors/TemplateItem.h"

// Sets default values
ATemplateItem::ATemplateItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATemplateItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATemplateItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
