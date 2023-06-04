// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#include "Core/Actors/TemplateTimeAndWeatherSystem.h"

// Engine Includes 
#include "Components/SkyLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/SkyAtmosphereComponent.h"

// Sets default values
ATemplateTimeAndWeatherSystem::ATemplateTimeAndWeatherSystem() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	SetRootComponent(BillboardComponent);

	SkyLightComponent = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLightComponent"));
	SkyLightComponent->SetupAttachment(GetRootComponent());
	SkyLightComponent->bRealTimeCapture = true; // Enable real time capture 
	SkyLightComponent->bCloudAmbientOcclusion = true;

	SunDirectionalLightComponent = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("SunLightComponent"));
	SunDirectionalLightComponent->SetupAttachment(GetRootComponent());
	SunDirectionalLightComponent->SetAtmosphereSunLight(true);
	SunDirectionalLightComponent->ForwardShadingPriority = 1.f;
	SunDirectionalLightComponent->SetAtmosphereSunLightIndex(0);

	MoonDirectionalLightComponent = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("MoonLightComponent"));
	MoonDirectionalLightComponent->SetupAttachment(GetRootComponent());
	MoonDirectionalLightComponent->SetAtmosphereSunLight(false);
	MoonDirectionalLightComponent->ForwardShadingPriority = 2.f;
	MoonDirectionalLightComponent->SetAtmosphereSunLightIndex(1);

	ExponentialHeightFogComponent = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("ExponentialHeightFog"));
	ExponentialHeightFogComponent->SetupAttachment(GetRootComponent());
	ExponentialHeightFogComponent->SetVolumetricFog(true);

	SkyAtmosphereComponent = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphereComponent"));
	SkyAtmosphereComponent->SetupAttachment(GetRootComponent());

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComponent->SetupAttachment(GetRootComponent());
	PostProcessComponent->BlendWeight = 0.f;
	PostProcessComponent->bUnbound = true;

	VolumetricCloudsComponent = CreateDefaultSubobject<UVolumetricCloudComponent>(TEXT("VolumetricCloudsComponent"));
	VolumetricCloudsComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ATemplateTimeAndWeatherSystem::BeginPlay() {
	Super::BeginPlay();
	InitializeTimecycle();

}

// Called every frame
void ATemplateTimeAndWeatherSystem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	UpdateTimeline(DeltaTime);
}

void ATemplateTimeAndWeatherSystem::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	InitializeTimecycle();
}

void ATemplateTimeAndWeatherSystem::InitializeTimecycle() {
	if (!SunDirectionalLightComponent || !MoonDirectionalLightComponent) { GEngine->AddOnScreenDebugMessage(-1 , 0.f , FColor::Red , "Sun Or Moon Directional Light Not Valid!" __FUNCTION__) ; return; }
	if (CF_Timecycle) {
		// Initialize Timecycle day night by binding the timeline functions 
		FOnTimelineFloat Timecycle_UpdateTimeline;
		FOnTimelineEvent Timecycle_FinishTimeline;

		Timecycle_UpdateTimeline.BindUFunction(this , "UpdateTimecycle");
		Timecycle_FinishTimeline.BindUFunction(this , "FinishedTimecycle");

		Timeline_Timecycle.AddInterpFloat(CF_Timecycle , Timecycle_UpdateTimeline);
		Timeline_Timecycle.SetTimelineFinishedFunc(Timecycle_FinishTimeline);
	}

	if (CF_Lighting) {
		// Initialize the lighting intensity curve and bind to timecycle timeline
		FOnTimelineFloat Lighting_UpdateTimeline;
		Lighting_UpdateTimeline.BindUFunction(this , "UpdateLighting");

		Timeline_Timecycle.AddInterpFloat(CF_Lighting , Lighting_UpdateTimeline);
	}

	if (CF_SkyLighting) {
		// Initialize the sky lighting intensity adding the curve and binding to the timecycle timeline
		FOnTimelineFloat SkyLighting_UpdateTimeline;
		SkyLighting_UpdateTimeline.BindUFunction(this , "UpdateSkyLighting");
		Timeline_Timecycle.AddInterpFloat(CF_SkyLighting , SkyLighting_UpdateTimeline);
	}

	if (CF_Temperature) {
		// Initialize the Temperature base value adding the curve and binding to the timecycle timeline
		FOnTimelineFloat Temperature_UpdateTimeline;
		Temperature_UpdateTimeline.BindUFunction(this , "UpdateTemperature");
		Timeline_Timecycle.AddInterpFloat(CF_Temperature , Temperature_UpdateTimeline);
	}

	Timeline_Timecycle.SetLooping(true);
	Timeline_Timecycle.Play();
	Timeline_Timecycle.SetPlayRate(TimecycleDayRate);
	Timeline_Timecycle.SetPlaybackPosition(TimecycleInitialTime * 100.f , true , true);

	const double TimelinePosition { Timeline_Timecycle.GetPlaybackPosition() };
	const double NewSunRotation { CF_Timecycle ? CF_Timecycle->GetFloatValue(TimelinePosition) : 0.f };

	double NewTime { 0.f };

	UKismetMathLibrary::FMod(NewSunRotation , 360.f , NewTime);

	SunDirectionalLightComponent->SetRelativeRotation(FRotator(NewTime + SunOffsetRotation.Pitch , SunOffsetRotation.Yaw , 0.f));
	MoonDirectionalLightComponent->SetRelativeRotation(FRotator((NewTime + SunOffsetRotation.Pitch) * -1.f , SunOffsetRotation.Yaw , 0.f));
}

void ATemplateTimeAndWeatherSystem::UpdateTimeline(const float& DeltaTime) {
	Timeline_Timecycle.TickTimeline(DeltaTime);
}

void ATemplateTimeAndWeatherSystem::UpdateTimecycle() {
	if (!SunDirectionalLightComponent || !MoonDirectionalLightComponent) { GEngine->AddOnScreenDebugMessage(-1 , 0.f , FColor::Red , "Sun Or Moon Directional Light Component Is Not Valid!" __FUNCTION__) ; return; }

	const double TimelinePosition { Timeline_Timecycle.GetPlaybackPosition() };
	const double NewSunRotation { CF_Timecycle->GetFloatValue(TimelinePosition) };

	double NewTime { 0.f };

	UKismetMathLibrary::FMod(NewSunRotation , 360.f , NewTime);

	SunDirectionalLightComponent->SetRelativeRotation(FRotator(NewTime + SunOffsetRotation.Pitch , SunOffsetRotation.Yaw , 0.f));
	MoonDirectionalLightComponent->SetRelativeRotation(FRotator((NewTime + SunOffsetRotation.Pitch) * -1.f , SunOffsetRotation.Yaw , 0.f));

	// Get time / hour / minutes / seconds
	// Convert the timeline length to 24:00 hour clock / time
	// Timeline time should be value of 2400.f on time and 360 on value for sun rotation  

	TimecycleHour = TimelinePosition / 100.f;
	TimecycleMinutes = (FMath::Fmod(TimelinePosition / (2400.f / 1440.f) , 60.f));
	TimecycleSeconds = (FMath::Fmod(TimelinePosition / ((2400.f / 1440.f) / 60.f) , 60.f));

	// Check if the current time is PM or AM 
	bIsPM = TimecycleHour >= 12 && TimecycleMinutes >= 0;
	// Get if it's currently night time 
	bIsNightTime = TimecycleHour > 18 && TimecycleHour <= 6;

	if (bShowTimecycleDebug) {
		GEngine->AddOnScreenDebugMessage(-1 , 0.f , FColor::Red , FString("Time : ") + FString::FromInt(TimecycleHour) + FString(" : ") + FString::FromInt(TimecycleMinutes) + FString(" : ") + FString::FromInt(TimecycleSeconds)) ;
	}
}

void ATemplateTimeAndWeatherSystem::FinishedTimecycle() {

	// Passed a day... 
	//DaysPassed++;
	// Increment the amount of days passed in-game
	DaysPassed++;
	if (bShowTimecycleDebug) {
		GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , "Passed A Day!!!");
	}
}

void ATemplateTimeAndWeatherSystem::UpdateLighting() {
	if (!SunDirectionalLightComponent || !MoonDirectionalLightComponent) { GEngine->AddOnScreenDebugMessage(-1 , 0.f , FColor::Red , "Sun Or Moon Directional Light Not Valid!" __FUNCTION__) ; return; }

	const double TimelinePosition { Timeline_Timecycle.GetPlaybackPosition() };
	const double NewSunLighting { CF_Lighting->GetFloatValue(TimelinePosition) };

	double NewLighting { 0.f };

	UKismetMathLibrary::FMod(NewSunLighting , 360.f , NewLighting);
	SunDirectionalLightComponent->SetIntensity(NewLighting);
	MoonDirectionalLightComponent->SetIntensity(NewLighting * -1.f); // Invert the sun intensity for the moon
}

void ATemplateTimeAndWeatherSystem::UpdateSkyLighting() {
	if (!SkyLightComponent) { GEngine->AddOnScreenDebugMessage(-1 , 0.f , FColor::Red , "Sky Light Component Not Valid!" __FUNCTION__) ; return; }

	const double TimelinePosition { Timeline_Timecycle.GetPlaybackPosition() };
	const double NewSunLighting { CF_Lighting->GetFloatValue(TimelinePosition) };

	double NewLighting { 0.f };

	UKismetMathLibrary::FMod(NewSunLighting , 360.f , NewLighting);
	SkyLightComponent->SetIntensity(NewLighting);
}

void ATemplateTimeAndWeatherSystem::UpdateTemperature() {
	const double TimelinePosition { Timeline_Timecycle.GetPlaybackPosition() };
	const double NewTemperatureBase { CF_Temperature->GetFloatValue(TimelinePosition) };

	double NewTemperatureMod { 0.f };

	UKismetMathLibrary::FMod(NewTemperatureBase , 360.f , NewTemperatureMod);

	// TODO Get weather temperature multipliers/dividers
	// TODO Get world zone temperatures multipliers/dividers

	CurrentTemperature = NewTemperatureMod;
}