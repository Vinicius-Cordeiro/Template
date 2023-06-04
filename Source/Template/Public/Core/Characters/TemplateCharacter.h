// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/Data/Interfaces/TemplateInterface.h"
#include "TemplateCharacter.generated.h"

// Engine Forward Declarations 
class USkeletalMeshComponent;
class UGroomComponent;
struct FInputActionValue;
class USpringArmComponent;
class UChildActorComponent;
// Custom Forward Declarations 
class UTemplateInputConfig;
class UTemplateInputMapping;
class UTemplateAnimInst;
class ATemplatePlayerController;
class UTemplateEnhancedInputComponent;
class ATemplatePlayerCameraManager;
class ATemplateCamera;
class UTemplateGameInstance;
class UTemplateAbilitySystemComponent;
class UTemplateMovementAttributesSet;
class UTemplateStatsAttributesSet;
class UTemplateWorldAttributesSet;
class UTemplateGameplayEffect;
class UTemplateGameplayEffectData;
class ATemplateTimeAndWeatherSystem;
class UTemplateFootstepMapDataAsset;
class UTemplateRagdollComponent;

/**
* Template Character used as a starting point, for base character infrastructure that can be used on player and also npc's
*/
UCLASS()
class TEMPLATE_API ATemplateCharacter : public ACharacter , public ITemplateInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATemplateCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#if WITH_EDITOR
	/**
	 * Called when a property on this object has been modified externally
	 *
	 * @param PropertyThatChanged the property that was modified
	 */
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

	/**
	 * Called when this Pawn is possessed. Only called on the server (or in standalone).
	 * @param NewController The controller possessing this pawn
	 */
	virtual void PossessedBy(AController* NewController) override;

	/** Called when our Controller no longer possesses us. Only called on the server (or in standalone). */
	virtual void UnPossessed() override;

	/** Event fired when the character has just started jumping */
	virtual void OnJumped_Implementation() override;

	/** Called when the character's movement enters falling */
	virtual void Falling() override;

	/**
	 * Called upon landing when falling, to perform actions based on the Hit result. Triggers the OnLanded event.
	 * Note that movement mode is still "Falling" during this event. Current Velocity value is the velocity at the time of landing.
	 * Consider OnMovementModeChanged() as well, as that can be used once the movement mode changes to the new mode (most likely Walking).
	 *
	 * @param Hit Result describing the landing that resulted in a valid landing spot.
	 * @see OnMovementModeChanged()
	 */
	virtual void Landed(const FHitResult& Hit) override;

	/**
	 * Called when Character crouches. Called on non-owned Characters through bIsCrouched replication.
	 * @param	HalfHeightAdjust		difference between default collision half-height, and actual crouched capsule half-height.
	 * @param	ScaledHalfHeightAdjust	difference after component scale is taken in to account.
	 */
	virtual void OnStartCrouch(float HalfHeightAdjust , float ScaledHalfHeightAdjust) override;

	/**
	 * Called when Character stops crouching. Called on non-owned Characters through bIsCrouched replication.
	 * @param	HalfHeightAdjust		difference between default collision half-height, and actual crouched capsule half-height.
	 * @param	ScaledHalfHeightAdjust	difference after component scale is taken in to account.
	 */
	virtual void OnEndCrouch(float HalfHeightAdjust , float ScaledHalfHeightAdjust) override;

	/**
	 * Called from CharacterMovementComponent to notify the character that the movement mode has changed.
	 * @param	PrevMovementMode	Movement mode before the change
	 * @param	PrevCustomMode		Custom mode before the change (applicable if PrevMovementMode is Custom)
	 */
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode , uint8 PreviousCustomMode = 0) override;

private:
	// boolean to check if the ability system has been initialized or not 
	bool bWasGameplayAbilityInitialized { false };
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = Input , meta = (AllowPrivateAccess = true , DisplayName = "Turn Rate"))
	float TurnRate { 10.f };

	/** Base look up rate, in deg/sec. Other scaling may affect final look up rate. */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = Input , meta = (AllowPrivateAccess = true , DisplayName = "Look Up Rate"))
	float LookUpRate { 10.f };

	/**Check if the player is looking at the character by holding the camera orbit key which will stop character rotation together with the camera control rotation*/
	bool bShouldUpdateCameraOrbit { false };

	/** Function to tell if the shoulder should be changed updating which side the camera is  */
	bool bShouldUpdateCameraShoulder { false };

	FVector TargetShoulderOffset { FVector(0.f,25.f ,25.f) };

	bool bRightShoulderCamera { true };

	/** Target distance for the camera boom reach */
	float TargetCameraDistance { 200.f };

	/** Min camera distance to clamp the values of the camera boom */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = Input , meta = (AllowPrivateAccess = true))
	float MinCameraDistance { 200.f };

	/** Max Camera distance to clamp the value of the camera boom */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Input , meta =(AllowPrivateAccess = true))
	float MaxCameraDistance { 500.f };

	/** Speed which the camera distance will change after a change on distance using input */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = Input , meta = (AllowPrivateAccess = true))
	float CameraInterpolationSpeed { 10.f };

	/** Speed which the camera shoulder offset will change after change using input */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = Input , meta = (AllowPrivateAccess = true))
	float CameraShoulderChangeInterpolationSpeed { 10.f };


	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = Input , meta = (AllowPrivateAccess = true))
	float LandAdditiveDivisor { -1200.f };

	/** Last velocity which the character was before started sprinting , so when the character stop spriting will be back to this velocity */
	float LastNonSprintSpeed { 150.f };
	float LastNonSprintSpeedCrouched { 75.f };

	/** Input Config Used To bind enhanced input data to the game */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = Input , meta = (AllowPrivateAccess = true , DisplayName = "Input Config"))
	TObjectPtr<UTemplateInputConfig> InputConfig;

	/** Input Mapping Used To bind enhanced input data to the game */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = Input , meta = (AllowPrivateAccess = true , DisplayName = "Input Map"))
	TObjectPtr<UTemplateInputMapping> InputMapping;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = Mapping , meta = (AllowPrivateAccess = true , DisplayName = "Footstep Mapping"))
	TObjectPtr<UTemplateFootstepMapDataAsset> FootstepMapping;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = Components , meta = (AllowPrivateAccess = true , DisplayName = "SpringArm"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = Components , meta = (AllowPrivateAccess = true , DisplayName = "Camera Actor Child Comp"))
	TObjectPtr<UChildActorComponent> CameraComponent;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = Components , meta =(AllowPrivateAccess = true , DisplayName = "Head"))
	TObjectPtr<USkeletalMeshComponent> HeadMeshComponent;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = Components , meta = (AllowPrivateAccess = true , DisplayName = "Hair"))
	TObjectPtr<UGroomComponent> HairGroomComponent;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = Components , meta = (AllowPrivateAccess = true , DisplayName = "EyeBrows"))
	TObjectPtr<UGroomComponent> EyebrowsGroomComponent;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = Components , meta = (AllowPrivateAccess = true , DisplayName = "Eyelashes"))
	TObjectPtr<UGroomComponent> EyelashesGroomComponent;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = Components , meta = (AllowPrivateAccess = true , DisplayName = "Mustache"))
	TObjectPtr<UGroomComponent> MustacheGroomComponent;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = Components , meta = (AllowPrivateAccess = true , DisplayName = "Beard"))
	TObjectPtr<UGroomComponent> BeardGroomComponent;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = Components , meta = (AllowPrivateAccess = true , DisplayName = "BodyHair"))
	TObjectPtr<UGroomComponent> BodyHairGroomComponent;

	/** Accessories for character customization ¬ */

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = Components , meta =(AllowPrivateAccess = true , DisplayName = "Top"))
	TObjectPtr<USkeletalMeshComponent> TopMeshComponent;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = Components, meta = (AllowPrivateAccess = true, DisplayName = "Bottom"))
	TObjectPtr<USkeletalMeshComponent> BottomMeshComponent;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = Components, meta =(AllowPrivateAccess = true , DisplayName = "Shoes"))
	TObjectPtr<USkeletalMeshComponent> ShoesMeshComponent;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = Components , meta =(AllowPrivateAccess = true , DisplayName = "Hat"))
	TObjectPtr<USkeletalMeshComponent> HatMeshComponent;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = Components , meta = (AllowPrivateAccess = true , DisplayName = "Glasses"))
	TObjectPtr<USkeletalMeshComponent> GlassesMeshComponent;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = Components , meta = (AllowPrivateAccess = true , DisplayName = "ArmorVest"))
	TObjectPtr<USkeletalMeshComponent> ArmorVestMeshComponent;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = Components , meta =(AllowPrivateAccess = true , DisplayName = "Gloves"))
	TObjectPtr<USkeletalMeshComponent> GlovesMeshComponent;
	
	/** Audio for the character Footstep, has inside parameter that allow control over modulation of the sound */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = "Components" , meta = (AllowPrivateAccess = true , DisplayName = "Footstep Audio"))
	TObjectPtr<UAudioComponent> FootstepAudioComponent;


	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = Abilities,meta = (AllowPrivateAccess = true))
	TObjectPtr<UTemplateAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = Abilities,meta=(AllowPrivateAccess = true))
	TObjectPtr<UTemplateRagdollComponent> RagdollComponent;

	/** Character Movement Attribute set , attributes such walk speed,current min max, fall velocity, landing force */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = Abilities , meta = (AllowPrivateAccess = true))
	TObjectPtr<UTemplateMovementAttributesSet> MovementAttributeSet;

	/** Character Stats Attribute set with health,nutrition,hydration,sleep , stamina */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = Abilities , meta = (AllowPrivateAccess = true))
	TObjectPtr<UTemplateStatsAttributesSet> StatsAttributeSet;

	/** World Attribute Set */
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadOnly , Category = Abilities , meta = (AllowPrivateAccess = true))
	TObjectPtr<UTemplateWorldAttributesSet> WorldAttributeSet;

	/** Input Config Used To bind enhanced input data to the game */
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<UTemplateGameplayEffectData> GameplayEffectsMapAsset;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = Animations,meta=(AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> LandingAdditiveMontage;

	UPROPERTY()
	TObjectPtr<ATemplatePlayerController> PlayerController { nullptr };
	UPROPERTY()
	TObjectPtr<UTemplateEnhancedInputComponent> EnhancedInputComponent { nullptr };
	UPROPERTY()
	TObjectPtr<ATemplatePlayerCameraManager> PlayerCameraManager { nullptr };
	UPROPERTY()
	TObjectPtr<ATemplateCamera> CameraActor { nullptr };
	UPROPERTY()
	TObjectPtr<UTemplateGameInstance> GameInstance { nullptr };

	UPROPERTY()
	TObjectPtr<ATemplateTimeAndWeatherSystem> WeatherManager { nullptr };
public:

	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetHead() { return HeadMeshComponent; }
	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetTop() { return TopMeshComponent; }
	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetBottom() { return BottomMeshComponent; }
	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetShoes() { return ShoesMeshComponent; }
	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetHat() { return HatMeshComponent; }
	FORCEINLINE TObjectPtr<UGroomComponent> GetHairGroom() { return HairGroomComponent; }
	FORCEINLINE TObjectPtr<UGroomComponent> GetEyebrowsGroom() { return EyebrowsGroomComponent; }
	FORCEINLINE TObjectPtr<UGroomComponent> GetEyelashesGroom() { return EyelashesGroomComponent; }
	FORCEINLINE TObjectPtr<UGroomComponent> GetMustacheGroom() { return MustacheGroomComponent; }
	FORCEINLINE TObjectPtr<UGroomComponent> GetBeardGroom() { return BeardGroomComponent; }
	FORCEINLINE TObjectPtr<UGroomComponent> GetBodyHairGroom() { return BodyHairGroomComponent; }
	FORCEINLINE TObjectPtr<USpringArmComponent> GetCameraBoom() { return SpringArmComponent; }
	FORCEINLINE TObjectPtr<ATemplateCamera> GetCameraActor();
	FORCEINLINE TObjectPtr<UTemplateAnimInst> GetAnimInst();
	FORCEINLINE TObjectPtr<UTemplateAbilitySystemComponent> GetAbilitySystemComponent(){ return AbilitySystemComponent; }
	FORCEINLINE TObjectPtr<UTemplateStatsAttributesSet> GetStats() { return StatsAttributeSet; }
	FORCEINLINE TObjectPtr<UTemplateMovementAttributesSet> GetMovementAttributes() { return MovementAttributeSet; }
	FORCEINLINE TObjectPtr<UTemplateWorldAttributesSet> GetWorldAttributes() { return WorldAttributeSet; }
	FORCEINLINE TObjectPtr<UTemplateFootstepMapDataAsset> GetFootstepDataAsset() { return FootstepMapping; }
	FORCEINLINE TObjectPtr<UAudioComponent> GetFootstepAudio() { return FootstepAudioComponent; }

	
	static TSubclassOf<UTemplateGameplayEffect> GetGameplayEffectByTag(UTemplateGameplayEffectData* InGameplayEffectsDataAsset , const FGameplayTag& InTag);
	bool ApplyEffectByTag(const FGameplayTag& InTag);
	
	/** Handle the velocity changes for the character movement speed, from walk to jog with the increase speed input */
	UFUNCTION()
	virtual void HandleMovementSpeedChange();

	/** Handle changes when the character stamina should be consumed or increased */
	UFUNCTION()
	virtual void HandleStaminaChange(const float& InStamina);

	UFUNCTION()
	virtual void HandleHealthChange(const float& InHealth);

	/** Returns current movement speed */
	UFUNCTION(BlueprintCallable)
	virtual float GetMoveSpeed() const;

	virtual void InitializeCharacter();
private:

	/** Handles moving forward/backward */
	void Input_Move(const FInputActionValue& InputActionValue);

	/** Handles mouse and stick look */
	void Input_Look(const FInputActionValue& InputActionValue);

	/** Handles Jumping */
	void Input_Jump(const FInputActionValue& InputActionValue);

	/** Handles Interactions with fire input button  */
	void Input_Fire(const FInputActionValue& InputActionValue);

	/** Handle on which shoulder the camera is placed on third person */
	void Input_ChangeShoulder(const FInputActionValue& InputActionValue);

	/** Handles Aim down sights function*/
	void Input_ADS(const FInputActionValue& InputActionValue);

	/** Handles the character crouching and uncrouching functionality */
	void Input_Crouch(const FInputActionValue& InputActionValue);

	/** Handles character rolling/dodge on the 8-axis directions */
	void Input_Roll(const FInputActionValue& InputActionValue);

	/** Handles the input for the character sprint action */
	void Input_Sprint(const FInputActionValue& InputActionValue);

	/** Handles the input for the character decrease on velocity */
	void Input_DecreaseSpeed(const FInputActionValue& InputActionValue);

	/** Handles the input for the character increase on velocity */
	void Input_IncreaseSpeed(const FInputActionValue& InputActionValue);

	/** Handles the input for the character decrease on velocity */
	void Input_DecreaseCameraDistance(const FInputActionValue& InputActionValue);

	/** Handles the input for the character increase on velocity */
	void Input_IncreaseCameraDistance(const FInputActionValue& InputActionValue);

	/** Handles the camera to orbit around the character when an input is pressed */
	void Input_OrbitCharacter(const FInputActionValue& InputActionValue);

	/**
	*	Function to change the velocity of the character to start sprinting , checking if it has enough stamina to continue sprinting being called on input call or 
	*	when the stamina changes with the HandleStaminaChange function... 
	*/
	void StartSprint();
	/**
	*	Function to change the velocity of the character to stop sprinting called when the character runs off stamina or input is released
	*/
	void StopSprint();
	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Update for the camera , and camera boom , and how it will feel while playing  */
	void UpdateCamera(const float& DeltaTime);
	void UpdateSpeed(const float& DeltaTime);

public:


	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Debug")
	bool bShowIsAliveConditionValue { true };
	/** Function used to know if the character is still alive, when this is called */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual bool IsAlive();


	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Debug")
	bool bShowHealthValue { true };
	/** Function used to get the health value from the attributes on the character */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual float GetHealth();
	/** Function used to get the max health value from the attributes on the character */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual float GetMaxHealth();


	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Debug")
	bool bShowStaminaValue { true };
	/** Function used to get the stamina value from the attributes on the character */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual float GetStamina();
	/** Function used to get the max stamina value from the attributes on the character */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual float GetMaxStamina();


	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Debug")
	bool bShowNutritionValue { true };
	/** Function used to get the Nutrition value from the attributes on the character */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual float GetNutrition();
	/** Function used to get the max Nutrition value from the attributes on the character */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual float GetMaxNutrition();


	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Debug")
	bool bShowHydrationValue { true };
	/** Function used to get the Hydration value from the attributes on the character */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual float GetHydration();
	/** Function used to get the max Hydration value from the attributes on the character */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual float GetMaxHydration();


	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Debug")
	bool bShowFatigueValue { true };
	/** Function used to get the Fatigue value from the attributes on the character */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual float GetFatigue();
	/** Function used to get the max Fatigue value from the attributes on the character */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual float GetMaxFatigue();


	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Debug")
	bool bShowDamageValue { false };
	/** Function used to know if the character is still alive, when this is called */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual float GetDamage();


	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Debug")
	bool bShowZVelocityValue { false };
	/** Function used to know if the character is still alive, when this is called */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual float GetZVelocity();


	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Debug")
	bool bShowDamageHeightThresholdValue { false };
	/** Function used to know if the character is still alive, when this is called */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual float GetDamageHeightThreshold();

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Debug")
	bool bShowTimecycleRateValue { true };
	/** Function used to know if the character is still alive, when this is called */
	UFUNCTION(BlueprintPure , Category = "Abilities")
	virtual float GetTimecycleRate();

	UFUNCTION(BlueprintPure , Category = "UserInteface")
	virtual FText GetCurrentTime();

	UFUNCTION(BlueprintPure , Category = "UserInteface")
	virtual FText GetCurrentWeather();

	virtual void DisplayDebugVariables(float DeltaTime);
};
