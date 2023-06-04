// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Data/Interfaces/TemplateInterface.h"
#include "TemplateTimeAndWeatherSystem.generated.h"

class UDirectionalLightComponent;
class UExponentialHeightFogComponent;
class UPostProcessComponent;
class UVolumetricCloudComponent;
class UTimelineComponent;


UCLASS()
class TEMPLATE_API ATemplateTimeAndWeatherSystem : public AActor, public ITemplateInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATemplateTimeAndWeatherSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;
protected:

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = "Components" , meta = (DisplayName = "Root Component"))
	TObjectPtr<UBillboardComponent> BillboardComponent;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = "Components" , meta = (DisplayName = "SkyLight"))
	TObjectPtr<USkyLightComponent> SkyLightComponent;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = "Components" , meta = (DisplayName = "Sun"))
	TObjectPtr<UDirectionalLightComponent> SunDirectionalLightComponent { nullptr };

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = "Components" , meta = (DisplayName = "Moon"))
	TObjectPtr<UDirectionalLightComponent> MoonDirectionalLightComponent { nullptr };

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = "Components" , meta = (DisplayName = "Exponential Fog"))
	TObjectPtr<UExponentialHeightFogComponent> ExponentialHeightFogComponent { nullptr };

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = "Components" , meta = (DisplayName = "Sky Atmosphere"))
	TObjectPtr<USkyAtmosphereComponent> SkyAtmosphereComponent { nullptr };

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = "Components" , meta = (DisplayName = "Post Process"))
	TObjectPtr<UPostProcessComponent> PostProcessComponent { nullptr };

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = "Components" , meta = (DisplayName = "Volumetric Clouds"))
	TObjectPtr<UVolumetricCloudComponent> VolumetricCloudsComponent { nullptr };

protected:

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Timecycle" , meta = (DisplayName = "Timecycle Curve"))
	TObjectPtr<UCurveFloat> CF_Timecycle { nullptr };

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Timecycle" , meta = (DisplayName = "Lighting Curve"))
	TObjectPtr<UCurveFloat> CF_Lighting { nullptr };

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Timecycle" , meta = (DisplayName = "Sky Lighting Curve"))
	TObjectPtr<UCurveFloat> CF_SkyLighting { nullptr };

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Timecycle" , meta = (DisplayName = "Temperature Curve"))
	TObjectPtr<UCurveFloat> CF_Temperature { nullptr };

	FTimeline Timeline_Timecycle;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Timecycle" , meta = (DisplayName = "Sun Offset"))
	FRotator SunOffsetRotation { FRotator(90 , 0 , 0) };

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Timecycle" , meta = (DisplayName = "Timecycle Rate" , ClampMin = 0.024f , ClampMax = 24.f))
	float TimecycleDayRate { 1.f };

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Timecycle" , meta = (DisplayName = "Initial Time" , ClampMin = 0.f , ClampMax = 24.f))
	float TimecycleInitialTime { 0.f };

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Timecycle" , meta = (DisplayName = "Hour" , ClampMin = 0 , ClampMax = 24))
	int32 TimecycleHour { 0 };

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Timecycle" , meta = (DisplayName = "Minutes" , ClampMin = 0 , ClampMax = 60))
	int32 TimecycleMinutes { 0 };

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Timecycle" , meta = (DisplayName = "Seconds" , ClampMin = 0 , ClampMax = 60))
	int32 TimecycleSeconds { 0 };

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Timecycle" , meta = (DisplayName = "Days Passed"))
	int32 DaysPassed { 0 };

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite , Category = "Timecycle")
	bool bIsNightTime { false };

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Timecycle")
	bool bIsPM { false };

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Timecycle")
	float CurrentTemperature { 30.f };

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Timecycle")
	bool bShowTimecycleDebug { false };


public:
	UFUNCTION(BlueprintCallable , Category = "Timecycle")
	void InitializeTimecycle();

	UFUNCTION(BlueprintCallable , Category = "Timecycle")
	void UpdateTimeline(const float& DeltaTime);

	UFUNCTION(BlueprintCallable , Category = "Timecycle")
	void UpdateTimecycle();

	UFUNCTION(BlueprintCallable , Category = "Timecycle")
	void FinishedTimecycle();

	UFUNCTION(BlueprintCallable , Category = "Timecycle")
	void UpdateLighting();

	UFUNCTION(BlueprintCallable , Category = "Timecycle")
	void UpdateSkyLighting();

	UFUNCTION(BlueprintCallable , Category = "Timecycle")
	void UpdateTemperature();

	UFUNCTION()
	FORCEINLINE float GetTimecycleRate() { return FMath::GetMappedRangeValueClamped(FVector2D(0.024f , 24.f) , FVector2D(0.001f,1.f),TimecycleDayRate);}

	UFUNCTION()
	FORCEINLINE int32 GetHour() { return TimecycleHour; }
	UFUNCTION()
	FORCEINLINE int32 GetMinutes() { return TimecycleMinutes; }
	UFUNCTION()
	FORCEINLINE int32 GetSeconds() { return TimecycleSeconds; }
	UFUNCTION()
	FORCEINLINE bool GetIsPM() { return bIsPM; }
	UFUNCTION()
	FORCEINLINE float GetTemperature() { return CurrentTemperature; }

};
