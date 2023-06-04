// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TemplatePhysicalMaterial.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATE_API UTemplatePhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()

public:
	UTemplatePhysicalMaterial(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// A container of gameplay tags that game code can use to reason about this physical material
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = PhysicalProperties)
	FGameplayTag Tag;
};
