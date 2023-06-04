// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Core/Characters/Animations/Notifies/TemplateAnimNotify.h"
#include "Core/Data/Interfaces/TemplateInterface.h"
#include "TemplateFootstepAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATE_API UTemplateFootstepAnimNotify : public UTemplateAnimNotify , public ITemplateInterface
{
	GENERATED_BODY()
public:
	UTemplateFootstepAnimNotify();

	virtual void Notify(USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = Footstep)
	class USoundBase* FootstepDefaultSound;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = Footstep)
	bool bShouldDrawDebug { false };
};
