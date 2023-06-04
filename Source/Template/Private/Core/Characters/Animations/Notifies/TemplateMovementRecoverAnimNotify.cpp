// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#include "Core/Characters/Animations/Notifies/TemplateMovementRecoverAnimNotify.h"

#include "Core/Characters/TemplateCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"


UTemplateMovementRecoverAnimNotify::UTemplateMovementRecoverAnimNotify() {
}

void UTemplateMovementRecoverAnimNotify::Notify(USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp , Animation , EventReference);

	if (!MeshComp || !Animation || (MeshComp && !MeshComp->GetOwner())) { return; }
	Character = Cast<ATemplateCharacter>(MeshComp->GetOwner());
	if (!Character) { return; }
	if (!Character->GetController()) { return; }
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	Character->GetController()->ResetIgnoreInputFlags();
}
