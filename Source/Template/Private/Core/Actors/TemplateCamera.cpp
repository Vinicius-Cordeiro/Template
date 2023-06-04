// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#include "Core/Actors/TemplateCamera.h"

#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
ATemplateCamera::ATemplateCamera() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CameraBoxCollision"));
	SetRootComponent(CameraCollisionComponent);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	CameraCollisionComponent->SetBoxExtent(FVector(4));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponentc"));
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->SetFieldOfView(90.f);

}

// Called when the game starts
void ATemplateCamera::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ATemplateCamera::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// Update Camera Field of view 
	UpdateCamera(DeltaTime);
}

void ATemplateCamera::ChangeCameraFOVInterpolated(float TargetFOV) {
	const float CurrentFieldOfView = GetCamera()->FieldOfView;

	if (CurrentFieldOfView != TargetFOV) {
		bShouldUpdateFieldOfView = true;
		TargetFieldOfView = TargetFOV;
	}
}

void ATemplateCamera::UpdateCamera(float DeltaTime) {
	if (!bShouldUpdateFieldOfView) return;
	const float CurrentFieldOfView = GetCamera()->FieldOfView;
	const float NewFOV = FMath::FInterpTo(CurrentFieldOfView , TargetFieldOfView , DeltaTime , CameraFOVInterpolationSpeed);

	GetCamera()->SetFieldOfView(NewFOV);

	// Check if the field of view has reached the target fov
	//if it has stop updating the fov 
	if (GetCamera()->FieldOfView == TargetFieldOfView) {
		bShouldUpdateFieldOfView = false;
	}

}

