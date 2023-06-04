// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#include "Core/Components/TemplateCharacterMovementComponent.h"

// Sets default values for this component's properties
UTemplateCharacterMovementComponent::UTemplateCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UTemplateCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called every frame
void UTemplateCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UTemplateCharacterMovementComponent::GetMaxSpeed() const {
	const float Vel { Super::GetMaxSpeed() };
	switch (MovementMode) {
		case MOVE_Walking:
		case MOVE_NavWalking:
			return IsCrouching() ? MaxWalkSpeedCrouched : MaxWalkSpeed;
		case MOVE_Falling:
			return MaxWalkSpeed;
		case MOVE_Swimming:
			return MaxSwimSpeed;
		case MOVE_Flying:
			return MaxFlySpeed;
		case MOVE_Custom:
			return MaxCustomMovementSpeed;
		case MOVE_None:
		default:
			return Vel;
	}
}
