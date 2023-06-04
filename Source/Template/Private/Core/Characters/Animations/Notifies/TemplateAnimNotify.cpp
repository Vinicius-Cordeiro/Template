// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#include "Core/Characters/Animations/Notifies/TemplateAnimNotify.h"

// Custom Includes
#include "Core/Characters/TemplateCharacter.h"

// Engine Includes
#include "Components/SkeletalMeshComponent.h"


void UTemplateAnimNotify::Notify(USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp , Animation , EventReference);
	checkf(MeshComp, TEXT("Not able to get character Animation Notify "));
	checkf(MeshComp->GetOwner(), TEXT("Not able to get character Mesh Comp Owner Animation Notify "));

	Character = Cast<ATemplateCharacter>(MeshComp->GetOwner());
}
