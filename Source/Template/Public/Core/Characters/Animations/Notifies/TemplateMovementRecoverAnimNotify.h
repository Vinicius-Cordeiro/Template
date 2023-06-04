// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Core/Characters/Animations/Notifies/TemplateAnimNotify.h"
#include "TemplateMovementRecoverAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATE_API UTemplateMovementRecoverAnimNotify : public UTemplateAnimNotify
{
	GENERATED_BODY()

public:

	UTemplateMovementRecoverAnimNotify();

	virtual void Notify(USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , const FAnimNotifyEventReference& EventReference) override;

};
