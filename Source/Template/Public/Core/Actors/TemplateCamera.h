// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TemplateCamera.generated.h"

class UCameraComponent;
class UBoxComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEMPLATE_API ATemplateCamera : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ATemplateCamera();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	/** Camera component which will give theview of the world to the player, can be modified with camera modifiers, fov modifiiers , as well as aspect ratio modifications */
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Camera" , meta = (TitleProperty = "Camera"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Camera" , meta = (TitleProperty = "CollisionBox"))
	TObjectPtr<UBoxComponent> CameraCollisionComponent;

	/** How fast the camera will interpolate to the target field of view after the call */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Camera" , meta = (ClampMin = 1.f , ClampMax = 10.f , DisplayName = "FOV Interpolation Speed"))
	float CameraFOVInterpolationSpeed { 4.f };

	float TargetFieldOfView { 90.f };

	bool bShouldUpdateFieldOfView { false };

public:

	FORCEINLINE TObjectPtr<UCameraComponent> GetCamera() { return CameraComponent; }

	FORCEINLINE TObjectPtr<UBoxComponent> GetCameraCollision() { return CameraCollisionComponent; }

	UFUNCTION(BlueprintCallable , Category = "Camera")
	virtual void ChangeCameraFOVInterpolated(float TargetFOV = 90.f);

	/** Function called to update the camera , could be collision, materials on the world the camera is colliding or the field of view change
	*	@param DeltaTime Game World/Engine World delta time
	*/
	UFUNCTION(BlueprintCallable , Category = "Camera")
	virtual void UpdateCamera(float DeltaTime);


};
