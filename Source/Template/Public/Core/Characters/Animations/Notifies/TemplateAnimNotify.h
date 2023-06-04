// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TemplateAnimNotify.generated.h"

class ATemplateCharacter;

UCLASS()
class TEMPLATE_API UTemplateAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(class USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY()
	TObjectPtr<ATemplateCharacter> Character;

};
