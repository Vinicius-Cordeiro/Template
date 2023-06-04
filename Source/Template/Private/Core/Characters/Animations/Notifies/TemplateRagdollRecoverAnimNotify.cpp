#include "Core/Characters/Animations/Notifies/TemplateRagdollRecoverAnimNotify.h"

#include "Core/Characters/TemplateCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Core/Data/Tags/TemplateGameplayTags.h"
#include "Core/Components/TemplateAbilitySystemComponent.h"

UTemplateRagdollRecoverAnimNotify::UTemplateRagdollRecoverAnimNotify() {
}

void UTemplateRagdollRecoverAnimNotify::Notify(USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp , Animation , EventReference);

	if (!MeshComp || !Animation || (MeshComp && !MeshComp->GetOwner())) { return; }
	Character = Cast<ATemplateCharacter>(MeshComp->GetOwner());
	if (!Character) { return; }
	if (!Character->GetController()) { return; }
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	Character->GetController()->ResetIgnoreInputFlags();

	if (UTemplateAbilitySystemComponent* ASC = Cast<UTemplateAbilitySystemComponent>(Character->GetAbilitySystemComponent())) {
		const FTemplateGameplayTags& GameplayTags { FTemplateGameplayTags::Get() };
		if (ASC->GetGameplayTagCount(GameplayTags.Tag_Ragdoll)) {
			ASC->SetLooseGameplayTagCount(GameplayTags.Tag_Ragdoll , 0);
		}
		if (ASC->GetGameplayTagCount(GameplayTags.Tag_Standing)) {
			ASC->SetLooseGameplayTagCount(GameplayTags.Tag_Standing , 0);
		}
	}
}

