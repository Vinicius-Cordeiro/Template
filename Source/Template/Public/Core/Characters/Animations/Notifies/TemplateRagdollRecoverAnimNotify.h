#pragma once

#include "CoreMinimal.h"
#include "Core/Characters/Animations/Notifies/TemplateAnimNotify.h"
#include "Core/Data/Interfaces/TemplateInterface.h"
#include "TemplateRagdollRecoverAnimNotify.generated.h"

UCLASS()
class TEMPLATE_API UTemplateRagdollRecoverAnimNotify : public UTemplateAnimNotify , public ITemplateInterface
{
	GENERATED_BODY()
public:
	UTemplateRagdollRecoverAnimNotify();
	virtual void Notify(USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , const FAnimNotifyEventReference& EventReference) override;

};
