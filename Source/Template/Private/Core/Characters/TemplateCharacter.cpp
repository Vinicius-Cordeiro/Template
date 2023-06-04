// All Rights Reserved. Copyrighted by Vinicius Cordeiro / Cerrado Game Studio. Contact: Email: vinicordeirogo@gmail.com ; Instagram: @vinicordeirogo ; Linkedin: vinicordeirox ; GitHub: Vinicius-Cordeiro


#include "Core/Characters/TemplateCharacter.h"

// Custom Includes
#include "Core/Components/TemplateCharacterMovementComponent.h"
#include "Core/Characters/Player/TemplatePlayerController.h"
#include "Core/Characters/Player/TemplatePlayerAIController.h"
#include "Core/Components/TemplateEnhancedInputComponent.h"
#include "Core/Data/Managers/TemplatePlayerCameraManager.h"
#include "Core/Actors/TemplateCamera.h"
#include "Core/Data/System/TemplateGameInstance.h"
#include "Core/Data/Input/TemplateInputMapping.h"
#include "Core/Data/Tags/TemplateGameplayTags.h"
#include "Core/Characters/Animations/Instances/TemplateAnimInst.h"
#include "Core/Components/TemplateAbilitySystemComponent.h"
#include "Core/Data/Attributes/TemplateWorldAttributesSet.h"
#include "Core/Data/Attributes/TemplateMovementAttributesSet.h"
#include "Core/Data/Attributes/TemplateStatsAttributesSet.h"
#include "Core/Data/GameplayEffects/TemplateGameplayEffectData.h"
#include "Core/Data/GameplayEffects/TemplateGameplayEffect.h"
#include "Core/Actors/TemplateTimeAndWeatherSystem.h"
#include "Core/Components/TemplateRagdollComponent.h"

// Engine Includes 
#include "Net/UnrealNetwork.h"
#include "EnhancedInput/Public/InputAction.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ChildActorComponent.h"
#include "GroomComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"


const FTemplateGameplayTags& GameplayTags { FTemplateGameplayTags::Get() };

ATemplateCharacter::ATemplateCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTemplateCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bWasGameplayAbilityInitialized = false;

	GetCharacterMovement()->RotationRate = FRotator(0 , 90 , 0);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->SetCrouchedHalfHeight(66.f);
	GetCharacterMovement()->bCanWalkOffLedges = true;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanSwim = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanJump = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 75.f;

	GetCapsuleComponent()->SetCapsuleRadius(22.f , true);

	static ConstructorHelpers::FClassFinder<ATemplatePlayerAIController> PlayerAIController = TEXT("/Game/Template/Core/Characters/Player/BP_PlayerIAController.BP_PlayerIAController_C");
	if (PlayerAIController.Succeeded()) AIControllerClass = PlayerAIController.Class;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0 , 0 , -88) , FRotator(0 , -90 , 0));
	GetMesh()->SetNotifyRigidBodyCollision(true);
	GetMesh()->SetCollisionProfileName("CharacterBodyCollision");
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera , ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMesh = TEXT("/Game/MetaHumans/m_nmr_avg_a1/Male/Medium/NormalWeight/Body/m_med_nrw_body.m_med_nrw_body");
	if (BodyMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(BodyMesh.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimationInstance = TEXT("/Game/Template/Core/Characters/Animations/AnimationInstances/AnimBP_TemplateCharacter.AnimBP_TemplateCharacter_C");
	if (AnimationInstance.Succeeded()) GetMesh()->SetAnimInstanceClass(AnimationInstance.Class);

	HeadMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMeshComponent"));
	HeadMeshComponent->SetupAttachment(GetMesh());
	HeadMeshComponent->SetLeaderPoseComponent(GetMesh() , true);
	HeadMeshComponent->SetCollisionProfileName("CharacterHeadCollision");
	HeadMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetHead()->SetCollisionResponseToChannel(ECC_Camera , ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HeadMesh = TEXT("/Game/MetaHumans/m_nmr_avg_a1/Face/m_nmr_avg_a1_FaceMesh.m_nmr_avg_a1_FaceMesh");
	if (HeadMesh.Succeeded()) {
		GetHead()->SetSkeletalMesh(HeadMesh.Object);
	}

	TopMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TopMeshComponent"));
	TopMeshComponent->SetupAttachment(GetMesh());
	TopMeshComponent->SetLeaderPoseComponent(GetMesh() , true);
	TopMeshComponent->SetCollisionProfileName("CharacterClothCollision");
	TopMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetTop()->SetCollisionResponseToChannel(ECC_Camera , ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TopMesh = TEXT("/Game/MetaHumans/Common/Male/Medium/NormalWeight/Tops/Hoodie/Meshes/m_med_nrw_top_hoodie_nrm.m_med_nrw_top_hoodie_nrm");
	if (TopMesh.Succeeded()) {
		GetTop()->SetSkeletalMesh(TopMesh.Object);
	}

	BottomMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BottomMeshComponent"));
	BottomMeshComponent->SetupAttachment(GetMesh());
	BottomMeshComponent->SetLeaderPoseComponent(GetMesh() , true);
	BottomMeshComponent->SetCollisionProfileName("CharacterClothCollision");
	BottomMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetBottom()->SetCollisionResponseToChannel(ECC_Camera , ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BottomMesh = TEXT("/Game/MetaHumans/Common/Male/Medium/NormalWeight/Bottoms/Cargopants/m_med_nrw_btm_cargopants.m_med_nrw_btm_cargopants");
	if (BottomMesh.Succeeded()) {
		GetBottom()->SetSkeletalMesh(BottomMesh.Object);
	}

	ShoesMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShoesMeshComponent"));
	ShoesMeshComponent->SetupAttachment(GetMesh());
	ShoesMeshComponent->SetLeaderPoseComponent(GetMesh() , true);
	ShoesMeshComponent->SetCollisionProfileName("CharacterClothCollision");
	ShoesMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetShoes()->SetCollisionResponseToChannel(ECC_Camera , ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ShoesMesh = TEXT("/Game/MetaHumans/Common/Male/Medium/NormalWeight/Shoes/CasualSneakers/m_med_nrw_shs_casualsneakers.m_med_nrw_shs_casualsneakers");
	if (ShoesMesh.Succeeded()) {
		GetShoes()->SetSkeletalMesh(ShoesMesh.Object);
	}

	HairGroomComponent = CreateDefaultSubobject<UGroomComponent>(TEXT("HairGroomComponent"));
	HairGroomComponent->SetupAttachment(GetHead());

	static ConstructorHelpers::FObjectFinder<UGroomAsset> HairStrands = TEXT("/Game/MetaHumans/m_nmr_hair_1/MaleHair/Hair/Hair_S_BuzzCut.Hair_S_BuzzCut");
	if (HairStrands.Succeeded()) HairGroomComponent->SetGroomAsset(HairStrands.Object);

	static ConstructorHelpers::FObjectFinder<UGroomBindingAsset> HairBindingAsset = TEXT(
		"/Game/MetaHumans/m_nmr_hair_1/MaleHair/GroomBinding/Hair_S_BuzzCut_m_head_Archetype_Binding.Hair_S_BuzzCut_m_head_Archetype_Binding");
	if (HairBindingAsset.Succeeded()) HairGroomComponent->SetBindingAsset(HairBindingAsset.Object);

	EyebrowsGroomComponent = CreateDefaultSubobject<UGroomComponent>(TEXT("EyebrowsGroomComponent"));
	EyebrowsGroomComponent->SetupAttachment(GetHead());

	static ConstructorHelpers::FObjectFinder<UGroomAsset> EyebrowsStrands = TEXT("/Game/MetaHumans/m_nmr_avg_a1/MaleHair/Hair/Eyebrows_M_Full.Eyebrows_M_Full");
	if (EyebrowsStrands.Succeeded()) EyebrowsGroomComponent->SetGroomAsset(EyebrowsStrands.Object);

	static ConstructorHelpers::FObjectFinder<UGroomBindingAsset> EyebrowsBindingAsset = TEXT(
		"/Game/MetaHumans/m_nmr_avg_a1/MaleHair/GroomBinding/Eyebrows_M_Full_m_head_Archetype_Binding.Eyebrows_M_Full_m_head_Archetype_Binding");
	if (EyebrowsBindingAsset.Succeeded()) EyebrowsGroomComponent->SetBindingAsset(EyebrowsBindingAsset.Object);

	EyelashesGroomComponent = CreateDefaultSubobject<UGroomComponent>(TEXT("EyelashesGroomComponent"));
	EyelashesGroomComponent->SetupAttachment(GetHead());

	static ConstructorHelpers::FObjectFinder<UGroomAsset> EyelashesStrands = TEXT("/Game/MetaHumans/m_nmr_avg_a1/MaleHair/Hair/Eyelashes_S_Thin.Eyelashes_S_Thin");
	if (EyelashesStrands.Succeeded()) EyelashesGroomComponent->SetGroomAsset(EyelashesStrands.Object);

	static ConstructorHelpers::FObjectFinder<UGroomBindingAsset> EyelashesBinding = TEXT(
		"/Game/MetaHumans/m_nmr_avg_a1/MaleHair/GroomBinding/Eyelashes_S_Thin_m_med_nrw_head_skmesh_Face_Archetype_Binding.Eyelashes_S_Thin_m_med_nrw_head_skmesh_Face_Archetype_Binding");
	if (EyelashesBinding.Succeeded()) EyelashesGroomComponent->SetBindingAsset(EyelashesBinding.Object);

	MustacheGroomComponent = CreateDefaultSubobject<UGroomComponent>(TEXT("MustacheGroomComponent"));
	MustacheGroomComponent->SetupAttachment(GetHead());

	static ConstructorHelpers::FObjectFinder<UGroomAsset> MustacheStrands = TEXT("/Game/MetaHumans/m_nmr_hair_5/MaleHair/Hair/Mustache_M_Curly.Mustache_M_Curly");
	if (MustacheStrands.Succeeded()) MustacheGroomComponent->SetGroomAsset(MustacheStrands.Object);

	static ConstructorHelpers::FObjectFinder<UGroomBindingAsset> MustacheBindingAsset = TEXT("/Game/MetaHumans/m_nmr_hair_5/MaleHair/GroomBinding/Mustache_M_Curly_m_head_Archetype_Binding.Mustache_M_Curly_m_head_Archetype_Binding");
	if (MustacheBindingAsset.Succeeded()) MustacheGroomComponent->SetBindingAsset(MustacheBindingAsset.Object);

	BeardGroomComponent = CreateDefaultSubobject<UGroomComponent>(TEXT("BeardGroomComponent"));
	BeardGroomComponent->SetupAttachment(GetHead());

	static ConstructorHelpers::FObjectFinder<UGroomAsset> BeardStands = TEXT("/Game/MetaHumans/m_nmr_hair_5/MaleHair/Hair/Beard_S_Curly.Beard_S_Curly");
	if (BeardStands.Succeeded()) BeardGroomComponent->SetGroomAsset(BeardStands.Object);

	static ConstructorHelpers::FObjectFinder<UGroomBindingAsset> BeardBindingAsset = TEXT("/Game/MetaHumans/m_nmr_hair_5/MaleHair/GroomBinding/Beard_S_Curly_m_med_nrw_head_skmesh_Binding.Beard_S_Curly_m_med_nrw_head_skmesh_Binding");
	if (BeardBindingAsset.Succeeded()) BeardGroomComponent->SetBindingAsset(BeardBindingAsset.Object);

	BodyHairGroomComponent = CreateDefaultSubobject<UGroomComponent>(TEXT("BodyHairGroomComponent"));
	BodyHairGroomComponent->SetupAttachment(GetMesh());

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmCameraBoomComp"));
	SpringArmComponent->SetupAttachment(GetMesh() , "Camera_Socket");
	SpringArmComponent->TargetArmLength = 200.f;
	SpringArmComponent->SocketOffset = FVector(0.f , 50.f , 25.f);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("CameraChildActorComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetChildActorClass(ATemplateCamera::StaticClass());


	static ConstructorHelpers::FObjectFinder<UTemplateInputConfig> InputConfigFinder = TEXT("/Game/Template/Core/Data/Input/TemplateInputConfig.TemplateInputConfig");
	if (InputConfigFinder.Succeeded()) InputConfig = InputConfigFinder.Object;

	static ConstructorHelpers::FObjectFinder<UTemplateInputMapping> InputMappingFinder = TEXT("/Game/Template/Core/Data/Input/TemplateInputMappingContext.TemplateInputMappingContext");
	if (InputMappingFinder.Succeeded()) InputMapping = InputMappingFinder.Object;

	AbilitySystemComponent = CreateDefaultSubobject<UTemplateAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	StatsAttributeSet = CreateDefaultSubobject<UTemplateStatsAttributesSet>(TEXT("StatsAttributes"));
	StatsAttributeSet->InitHealth(100.f);
	StatsAttributeSet->InitStamina(100.f);
	StatsAttributeSet->InitNutrition(100.f);
	StatsAttributeSet->InitHydration(100.f);
	StatsAttributeSet->InitFatigue(100.f);
	StatsAttributeSet->InitMaxHealth(100.f);
	StatsAttributeSet->InitMaxStamina(100.f);
	StatsAttributeSet->InitMaxNutrition(100.f);
	StatsAttributeSet->InitMaxHydration(100.f);
	StatsAttributeSet->InitMaxFatigue(100.f);

	MovementAttributeSet = CreateDefaultSubobject<UTemplateMovementAttributesSet>(TEXT("MovementAttributes"));
	MovementAttributeSet->InitCurrentVelocity(300.f);
	MovementAttributeSet->InitMinVelocity(150.f);
	MovementAttributeSet->InitMaxVelocity(300.f);

	WorldAttributeSet = CreateDefaultSubobject<UTemplateWorldAttributesSet>(TEXT("WorldAttributes"));
	WorldAttributeSet->InitWorldTimecycleRate(1.f);

	FootstepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootstepAudioComponent"));
	FootstepAudioComponent->SetAutoActivate(false);
	FootstepAudioComponent->SetVolumeMultiplier(0.f);

	RagdollComponent = CreateDefaultSubobject<UTemplateRagdollComponent>(TEXT("RagdollComponent"));
}

void ATemplateCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called when the game starts or when spawned
void ATemplateCharacter::BeginPlay() {
	Super::BeginPlay();
	// Initialize character when the game starts by pickin and storing variables,pointers,refs 
	InitializeCharacter();

}

// Called every frame
void ATemplateCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	// Update camera shoulder 
	UpdateCamera(DeltaTime);
	// Update speed of the character 
	UpdateSpeed(DeltaTime);
	// Show debug variables 
	DisplayDebugVariables(DeltaTime);
}

// Called to bind functionality to input
void ATemplateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	/** Try to obtain the player controller if was not already set if still fails return */
	if (!PlayerController && Cast<ATemplatePlayerController>(GetController())) {
		PlayerController = Cast<ATemplatePlayerController>(GetController());
		return;
	}

	/** Acquire the local player subsystem */
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	/** Clear out all  previous input mappings, than add our new mappings , updating basically on change runtime */
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping , 0);

	EnhancedInputComponent = Cast<UTemplateEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1 , 10.f , FColor::Red , "Unable to initialize Enhanced Input Component , check project settings!! \n " __FUNCTION__);
		return;
	}

	//Bind Input actions by tag
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_Move , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_Move);
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_Look_Mouse , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_Look);
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_Look_Stick , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_Look);
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_Fire , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_Fire);
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_Jump , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_Jump);
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_ChangeShoulder , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_ChangeShoulder);
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_ADS , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_ADS);
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_Crouch , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_Crouch);
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_Roll , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_Roll);
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_Sprint , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_Sprint);
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_IncreaseSpeed , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_IncreaseSpeed);
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_DecreaseSpeed , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_DecreaseSpeed);
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_IncreaseCameraDistance , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_IncreaseCameraDistance);
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_DecreaseCameraDistance , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_DecreaseCameraDistance);
	EnhancedInputComponent->BindActionByTag(InputConfig , GameplayTags.InputTag_OrbitCharacter , ETriggerEvent::Triggered , this , &ATemplateCharacter::Input_OrbitCharacter);
}

void ATemplateCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void ATemplateCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	InitializeCharacter();
}

void ATemplateCharacter::UnPossessed() {
	Super::UnPossessed();
}

void ATemplateCharacter::OnJumped_Implementation() {
	Super::OnJumped_Implementation();
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.Tag_Jumping , 1);
}

void ATemplateCharacter::Falling() {
	Super::Falling();
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.Tag_Falling , 1);
}

void ATemplateCharacter::Landed(const FHitResult& Hit) {
	Super::Landed(Hit);

	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.Tag_Jumping , 0);
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.Tag_Falling , 0);
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.Tag_Landing , 1);

	GetAnimInst()->LandingAdditiveAlpha = GetVelocity().Z / LandAdditiveDivisor;

	GetAnimInst()->Montage_Play(LandingAdditiveMontage , 1);
}

void ATemplateCharacter::OnStartCrouch(float HalfHeightAdjust , float ScaledHalfHeightAdjust) {
	Super::OnStartCrouch(HalfHeightAdjust , ScaledHalfHeightAdjust);
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.Tag_Crouching , 1);
}

void ATemplateCharacter::OnEndCrouch(float HalfHeightAdjust , float ScaledHalfHeightAdjust) {
	Super::OnEndCrouch(HalfHeightAdjust , ScaledHalfHeightAdjust);
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.Tag_Crouching , 0);
}

void ATemplateCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode , uint8 PreviousCustomMode) {
	Super::OnMovementModeChanged(PrevMovementMode , PreviousCustomMode);
}

TObjectPtr<UTemplateAnimInst> ATemplateCharacter::GetAnimInst() {
	return Cast<UTemplateAnimInst>(GetMesh()->GetAnimInstance());
}

TSubclassOf<UTemplateGameplayEffect> ATemplateCharacter::GetGameplayEffectByTag(UTemplateGameplayEffectData* InGameplayEffectsDataAsset , const FGameplayTag& InTag) {
	check(InGameplayEffectsDataAsset);
	if (const TSubclassOf<UTemplateGameplayEffect> GameplayEffect = InGameplayEffectsDataAsset->GetGameplayEffectByTag(InTag)) {
		return GameplayEffect;
	}
	return TSubclassOf<UTemplateGameplayEffect>();
}

bool ATemplateCharacter::ApplyEffectByTag(const FGameplayTag& InTag) {
	check(GameplayEffectsMapAsset);
	const TSubclassOf<UTemplateGameplayEffect> GameplayEffect = GetGameplayEffectByTag(GameplayEffectsMapAsset , InTag);
	if (GameplayEffect && GetAbilitySystemComponent()->GetGameplayTagCount(InTag) > 0) {
		const FGameplayEffectContextHandle NewHandle = GetAbilitySystemComponent()->MakeEffectContext();
		const FGameplayEffectSpecHandle GameplayEffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect , 1 , NewHandle);
		return GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get() , GetAbilitySystemComponent()).WasSuccessfullyApplied();
	}
	return false;
}


void ATemplateCharacter::HandleMovementSpeedChange() {
	// Update the character movement's walk speed
	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
	GetCharacterMovement()->MaxWalkSpeedCrouched = FMath::GetMappedRangeValueClamped(FVector2D(MovementAttributeSet->GetMinVelocity() , MovementAttributeSet->GetMaxVelocity()) , FVector2D(75.f , 125.f) , GetMoveSpeed());
}

void ATemplateCharacter::HandleStaminaChange(const float& InStamina) {
	if (InStamina <= 0.f) {
		StopSprint();
	}
}

void ATemplateCharacter::HandleHealthChange(const float& InHealth) {
}

float ATemplateCharacter::GetMoveSpeed() const {
	return MovementAttributeSet->GetCurrentVelocity();
}

void ATemplateCharacter::InitializeCharacter() {

	if (!bWasGameplayAbilityInitialized) {
		if (!GetAbilitySystemComponent()) return;
		GetAbilitySystemComponent()->InitAbilityActorInfo(this , this);
		check(GameplayEffectsMapAsset);
		for (TSubclassOf<UTemplateGameplayEffect> GameplayEffect : GameplayEffectsMapAsset->InitialGameplayEffectClasses) {
			if (!GameplayEffect) continue;
			// Only apply one of this gameplay effect 
			if (GetAbilitySystemComponent()->GetGameplayEffectCount(GameplayEffect , GetAbilitySystemComponent() , true) >= 1) continue;

			const FGameplayEffectContextHandle NewHandle = GetAbilitySystemComponent()->MakeEffectContext();
			const FGameplayEffectSpecHandle GameplayEffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect , 1 , NewHandle);

			GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get() , GetAbilitySystemComponent());
		}
		bWasGameplayAbilityInitialized = true;
	}
	if (!PlayerController)
		PlayerController = Cast<ATemplatePlayerController>(UGameplayStatics::GetPlayerController(this , 0));
	if (!PlayerCameraManager)
		PlayerCameraManager = Cast<ATemplatePlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(this , 0));
	if (!GameInstance)
		GameInstance = Cast<UTemplateGameInstance>(UGameplayStatics::GetGameInstance(this));
	// Bind the custom camera to the player controller 
	if (PlayerController && ((CameraComponent) && CameraComponent->GetChildActor())) {
		PlayerController->SetViewTarget(CameraComponent->GetChildActor());
		PlayerController->SetViewTarget(GetCameraActor());
	}

	if (PlayerController)
		PlayerController->SetViewTarget(GetCameraActor());

	// bind the landing delegate to the ragdoll component as well so we can check the damage and how would do it if the landing is hard
	if (RagdollComponent) {
		LandedDelegate.RemoveDynamic(RagdollComponent , &UTemplateRagdollComponent::OnLanded);
		LandedDelegate.AddDynamic(RagdollComponent , &UTemplateRagdollComponent::OnLanded);
	}

	// Set the default spring arm distance to the minimun camera distance 
	SpringArmComponent->TargetArmLength = MinCameraDistance;

	// Try to gather the game timecycle manager
	if (ATemplateTimeAndWeatherSystem * TimecycleManager { Cast<ATemplateTimeAndWeatherSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), ATemplateTimeAndWeatherSystem::StaticClass())) }) {
		WeatherManager = TimecycleManager;
		GetAbilitySystemComponent()->ApplyModToAttribute(Cast<UTemplateWorldAttributesSet>(GetAbilitySystemComponent()->GetAttributeSet(UTemplateWorldAttributesSet::StaticClass()))->GetWorldTimecycleRateAttribute() , EGameplayModOp::Override , WeatherManager->GetTimecycleRate());
	}

}

TObjectPtr<ATemplateCamera> ATemplateCharacter::GetCameraActor() {
	if ((CameraComponent) && CameraComponent->GetChildActor()) {
		CameraActor = Cast<ATemplateCamera>(CameraComponent->GetChildActor());
		if (!CameraActor) { return nullptr; }
	}
	return CameraActor;
}


void ATemplateCharacter::Input_Move(const FInputActionValue& InputActionValue) {
	if (!GetController()) return;

	/** Retrieve the value from the enhanced input */
	const FVector2D MoveValue = InputActionValue.Get<FVector2D>();

	// Retrieve the yaw rotation of the controller
	const FRotator YawRotation = FRotator(0 , GetController()->GetControlRotation().Yaw , 0);

	// get the direction which the character will move 
	// Forward/Backward Direction 
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// Left/Right Direction 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	/** Forward/Backward movement  */
	if (MoveValue.Y != 0.f)
		AddMovementInput(ForwardDirection , MoveValue.Y);

	/** Left/Right movement */
	if (MoveValue.X != 0.f)
		AddMovementInput(RightDirection , MoveValue.X);
}

void ATemplateCharacter::Input_Look(const FInputActionValue& InputActionValue) {
	if (!GetController()) return;

	/** Retrieve the value from the enhanced input */
	const FVector2D LookValue = InputActionValue.Get<FVector2D>();

	if (LookValue.X != 0.0f)
		TurnAtRate(LookValue.X);

	if (LookValue.Y != 0.0f)
		LookUpAtRate(LookValue.Y);
}

void ATemplateCharacter::Input_Jump(const FInputActionValue& InputActionValue) {
	if (!InputActionValue.Get<bool>()) return;
	Jump();
}

void ATemplateCharacter::Input_Fire(const FInputActionValue& InputActionValue) {
	if (!InputActionValue.Get<bool>()) return;
}

void ATemplateCharacter::Input_ChangeShoulder(const FInputActionValue& InputActionValue) {
	// Check whether the input has being pressed or not 
	if (!InputActionValue.Get<bool>()) return;
	
	// Only update the target offset if we are not already changing the socket offset otherwise will cause bug which the camera boom socket offset will not go all the way and will stay at the current value 
	bRightShoulderCamera = !bRightShoulderCamera;
	// change if the animations of the character should be mirrored this info will be sent to the animation instance 
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.Tag_MirrorAnimations , bRightShoulderCamera ? 0 : 1);
	TargetShoulderOffset = FVector(GetCameraBoom()->SocketOffset.X , bRightShoulderCamera ? 50.f : -50.f , GetCameraBoom()->SocketOffset.Z);
	bShouldUpdateCameraShoulder = true;
}

void ATemplateCharacter::Input_ADS(const FInputActionValue& InputActionValue) {
	if (!GetCameraActor()) return;
	InputActionValue.Get<bool>() ? GetCameraActor()->ChangeCameraFOVInterpolated(50.f) : GetCameraActor()->ChangeCameraFOVInterpolated(90.f);
}

void ATemplateCharacter::Input_Crouch(const FInputActionValue& InputActionValue) {
	//if (!InputActionValue.Get<bool>()) { return; }
	// Hold Crouch Button
	InputActionValue.Get<bool>() ? UnCrouch() : Crouch();
}

void ATemplateCharacter::Input_Roll(const FInputActionValue& InputActionValue) {
	if (!InputActionValue.Get<bool>()) { return; }
}

void ATemplateCharacter::Input_Sprint(const FInputActionValue& InputActionValue) {
	// HOLD SPRINT BUTTON
	(InputActionValue.Get<bool>()) ? StartSprint() : StopSprint();
}

void ATemplateCharacter::Input_DecreaseSpeed(const FInputActionValue& InputActionValue) {
	if (!InputActionValue.Get<bool>()) { GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.InputTag_DecreaseSpeed , 0); return; }
	if (GetAbilitySystemComponent()->GetGameplayTagCount(GameplayTags.InputTag_Sprint) >= 1) return;
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.InputTag_DecreaseSpeed , 1);
	ApplyEffectByTag(GameplayTags.InputTag_DecreaseSpeed);
}

void ATemplateCharacter::Input_IncreaseSpeed(const FInputActionValue& InputActionValue) {
	if (!InputActionValue.Get<bool>()) { GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.InputTag_IncreaseSpeed , 0); return; }
	if (GetAbilitySystemComponent()->GetGameplayTagCount(GameplayTags.InputTag_Sprint) >= 1) return;
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.InputTag_IncreaseSpeed , 1);
	ApplyEffectByTag(GameplayTags.InputTag_IncreaseSpeed);
}

void ATemplateCharacter::Input_DecreaseCameraDistance(const FInputActionValue& InputActionValue) {
	if (!InputActionValue.Get<bool>()) return;
	const float CameraDistance = GetCameraBoom()->TargetArmLength;
	TargetCameraDistance = FMath::Clamp(CameraDistance - 25.f , MinCameraDistance , MaxCameraDistance);
}

void ATemplateCharacter::Input_IncreaseCameraDistance(const FInputActionValue& InputActionValue) {
	if (!InputActionValue.Get<bool>()) return;
	const float CameraDistance = GetCameraBoom()->TargetArmLength;
	TargetCameraDistance = FMath::Clamp(CameraDistance + 25.f , MinCameraDistance , MaxCameraDistance);
}

void ATemplateCharacter::Input_OrbitCharacter(const FInputActionValue& InputActionValue) {
	bShouldUpdateCameraOrbit = (InputActionValue.Get<bool>());
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.InputTag_OrbitCharacter , bShouldUpdateCameraOrbit);
}

void ATemplateCharacter::StartSprint() {
	LastNonSprintSpeed = GetCharacterMovement()->MaxWalkSpeed;
	LastNonSprintSpeedCrouched = GetCharacterMovement()->MaxWalkSpeedCrouched;
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.Tag_Sprinting , 1);
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.InputTag_Sprint , 1);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 125.f;
}

void ATemplateCharacter::StopSprint() {
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.Tag_Sprinting , 0);
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.InputTag_Sprint , 0);
	GetCharacterMovement()->MaxWalkSpeed = LastNonSprintSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = LastNonSprintSpeedCrouched;
}

void ATemplateCharacter::TurnAtRate(float Rate) {
	// Starting to think about using the rate only when using controller, keyboard and mouse can become annoying when the fps drops below 30
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRate * GetWorld()->GetDeltaSeconds());
}

void ATemplateCharacter::LookUpAtRate(float Rate) {
	// Starting to think about using the rate only when using controller, keyboard and mouse can become annoying when the fps drops below 30
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATemplateCharacter::UpdateSpeed(const float& DeltaTime) {
	const bool bIsMoving { FMath::Abs(GetVelocity().Size()) > 0.f && !GetCharacterMovement()->IsFalling() };
	const bool bIsSprinting { bIsMoving && GetAbilitySystemComponent()->GetGameplayTagCount(GameplayTags.InputTag_Sprint) >= 1 };

	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.Tag_Sprinting , bIsSprinting);
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(GameplayTags.Tag_Moving , bIsMoving);

	const int32 GameplayEffectCount = GetAbilitySystemComponent()->GetGameplayEffectCount(GetGameplayEffectByTag(GameplayEffectsMapAsset , GameplayTags.Tag_Sprinting) , GetAbilitySystemComponent() , true);
	const TSubclassOf<UTemplateGameplayEffect> GameplayEffect = GetGameplayEffectByTag(GameplayEffectsMapAsset , GameplayTags.Tag_Sprinting);

	if (GameplayEffectCount == 0 && GameplayEffect) {
		// Only apply one of this gameplay effect 
		if (GetAbilitySystemComponent()->GetGameplayEffectCount(GameplayEffect , GetAbilitySystemComponent() , true) >= 1) return;
		const FGameplayEffectContextHandle NewHandle = GetAbilitySystemComponent()->MakeEffectContext();
		const FGameplayEffectSpecHandle GameplayEffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect , 1 , NewHandle);
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get() , GetAbilitySystemComponent());
	}
}

void ATemplateCharacter::UpdateCamera(const float& DeltaTime) {
	/** Camera orbit change update */
	if (bShouldUpdateCameraOrbit && FMath::Abs(GetVelocity().Size()) <= 0.f)
		GetCharacterMovement()->bUseControllerDesiredRotation = false , GetCameraBoom()->bUsePawnControlRotation = true;
	else
		GetCharacterMovement()->bUseControllerDesiredRotation = true , GetCameraBoom()->bUsePawnControlRotation = true;


	if (bShouldUpdateCameraShoulder) {
		const FVector CurrentOffset = GetCameraBoom()->SocketOffset;
		const FVector NewOffset = FMath::VInterpTo(CurrentOffset , TargetShoulderOffset , DeltaTime , CameraShoulderChangeInterpolationSpeed);
		GetCameraBoom()->SocketOffset = NewOffset;

		if (CurrentOffset == TargetShoulderOffset) bShouldUpdateCameraShoulder = false;
	}

	/** Camera distance Update */
	const float CameraDistance = GetCameraBoom()->TargetArmLength;

	if (CameraDistance != TargetCameraDistance) {
		GetCameraBoom()->TargetArmLength = FMath::FInterpTo(CameraDistance , TargetCameraDistance , DeltaTime , CameraInterpolationSpeed);
	}
}


/** Get If the character is still alive ! */
bool ATemplateCharacter::IsAlive() {
	if (!GetStats()) { return false; }
	if (GetStats()->GetHealth() <= 0.f) {
		// Start simulating physics for a dead body effect 
		GetMesh()->SetSimulatePhysics(true);
		// Ignore Movement input as well as 
		if (GetController()) GetController()->SetIgnoreMoveInput(true);
		FPoseSnapshot DeathSnapshot;
		GetAnimInst()->SnapshotPose(DeathSnapshot);
		DetachFromControllerPendingDestroy();
		return false;
	}
	return GetStats()->GetHealth() > 0.f;
}

float ATemplateCharacter::GetHealth() {
	if (!GetStats()) { return 0.f; }
	return GetStats()->GetHealth();
}

float ATemplateCharacter::GetMaxHealth() {
	if (!GetStats()) { return 0.f; }
	return GetStats()->GetMaxHealth();
}

float ATemplateCharacter::GetStamina() {
	if (!GetStats()) { return 0.f; }
	return GetStats()->GetStamina();
}

float ATemplateCharacter::GetMaxStamina() {
	if (!GetStats()) { return 0.f; }
	return GetStats()->GetMaxStamina();
}

float ATemplateCharacter::GetNutrition() {
	if (!GetStats()) { return 0.f; }
	return GetStats()->GetNutrition();
}

float ATemplateCharacter::GetMaxNutrition() {
	if (!GetStats()) { return 0.f; }
	return GetStats()->GetMaxNutrition();
}

float ATemplateCharacter::GetHydration() {
	if (!GetStats()) { return 0.f; }
	return GetStats()->GetHydration();
}

float ATemplateCharacter::GetMaxHydration() {
	if (!GetStats()) { return 0.f; }
	return GetStats()->GetMaxHydration();
}

float ATemplateCharacter::GetFatigue() {
	if (!GetStats()) { return 0.f; }
	return GetStats()->GetFatigue();
}

float ATemplateCharacter::GetMaxFatigue() {
	if (!GetStats()) { return 0.f; }
	return GetStats()->GetMaxFatigue();
}

float ATemplateCharacter::GetDamage() {
	if (!GetStats()) { return 0.f; }
	return 0.f;
}

float ATemplateCharacter::GetZVelocity() {
	if (!GetMovementAttributes()) { return 0.f; }
	return 0.f;
}

float ATemplateCharacter::GetDamageHeightThreshold() {
	return 0.f;
}

float ATemplateCharacter::GetTimecycleRate() {
	if (!GetWorldAttributes()) { return 0.f; }
	return GetWorldAttributes()->GetWorldTimecycleRate();
}

#define LOCTEXT_NAMESPACE "CHARACTER_KEYNAMESPACE"

FText ATemplateCharacter::GetCurrentTime() {
	if (!WeatherManager) return FText();
	FNumberFormattingOptions NumberFormatting;
	NumberFormatting.MaximumFractionalDigits = 0;
	NumberFormatting.MinimumIntegralDigits = 2;
	NumberFormatting.SetRoundingMode(HalfFromZero);

	const FText Time { FText::Format(LOCTEXT("Key_WeatherTime","{0}:{1}:{2}"),FText::AsNumber(WeatherManager->GetHour(),&NumberFormatting),FText::AsNumber(WeatherManager->GetMinutes(),&NumberFormatting),FText::AsNumber(WeatherManager->GetSeconds(),&NumberFormatting)) };
	return Time;
}

FText ATemplateCharacter::GetCurrentWeather() {
	if (!WeatherManager) return FText();
	FNumberFormattingOptions NumberFormatting;
	NumberFormatting.MaximumFractionalDigits = 0;
	NumberFormatting.SetRoundingMode(HalfFromZero);
	const FText WeatherText { FText::FromString("Clear") };
	const FText TemperatureText { FText::AsNumber(WeatherManager->GetTemperature(),&NumberFormatting) };
	const FText Weather { FText::Format(LOCTEXT("Key_Weather","{0}  -  {1} Cº"),WeatherText , TemperatureText) };
	return Weather;
}

void ATemplateCharacter::DisplayDebugVariables(float DeltaTime) {
	
	if (!GEngine || !GetAbilitySystemComponent() || !GetStats() || !GetMovementAttributes()) return;

	FNumberFormattingOptions NumberFormatting;
	NumberFormatting.MaximumFractionalDigits = 1;
	NumberFormatting.SetRoundingMode(HalfFromZero);

	/** Keep in mind the top line will appear at the bottom of the debug display !!! WARN TODO */

	if (bShowTimecycleRateValue) {
		const FText TimecycleRate { FText::Format(LOCTEXT("Key_TimecycleRate", "Timecycle Rate : {0}"), FText::AsNumber(GetTimecycleRate(), &NumberFormatting)) };
		GEngine->AddOnScreenDebugMessage(-1 , DeltaTime , FColor::Magenta , TimecycleRate.ToString());
		const FText WeatherText { FText::Format(LOCTEXT("Key_Weather", "Weather : {0}"), GetCurrentWeather()) };
		GEngine->AddOnScreenDebugMessage(-1 , DeltaTime , FColor::Magenta , WeatherText.ToString());
	}

	if (bShowFatigueValue) {
		const FText FatigueValue = FText::Format(LOCTEXT("Key_Fatigue" , "Fatigue : {0}/{1}") , FText::AsNumber(GetFatigue() , &NumberFormatting) , FText::AsNumber(GetMaxFatigue() , &NumberFormatting));
		GEngine->AddOnScreenDebugMessage(-1 , DeltaTime , FColor::Red , FatigueValue.ToString());
	}

	if (bShowHydrationValue) {
		const FText HydrationValue = FText::Format(LOCTEXT("Key_Hydration" , "Hydration : {0}/{1}") , FText::AsNumber(GetHydration() , &NumberFormatting) , FText::AsNumber(GetMaxHydration() , &NumberFormatting));
		GEngine->AddOnScreenDebugMessage(-1 , DeltaTime , FColor::Red , HydrationValue.ToString());
	}

	if (bShowNutritionValue) {
		const FText NutritionValue = FText::Format(LOCTEXT("Key_Nutrition" , "Nutrition : {0}/{1}") , FText::AsNumber(GetNutrition() , &NumberFormatting) , FText::AsNumber(GetMaxNutrition() , &NumberFormatting));
		GEngine->AddOnScreenDebugMessage(-1 , DeltaTime , FColor::Red , NutritionValue.ToString());
	}

	if (bShowStaminaValue) {
		const FText StaminaValue = FText::Format(LOCTEXT("Key_Stamina" , "Stamina : {0}/{1}") , FText::AsNumber(GetStamina() , &NumberFormatting) , FText::AsNumber(GetMaxStamina() , &NumberFormatting));
		GEngine->AddOnScreenDebugMessage(-1 , DeltaTime , FColor::Red , StaminaValue.ToString());
	}

	if (bShowHealthValue) {
		const FText HealthValue = FText::Format(LOCTEXT("Key_Health" , "Health : {0}/{1}") , FText::AsNumber(GetHealth() , &NumberFormatting) , FText::AsNumber(GetMaxHealth() , &NumberFormatting));
		GEngine->AddOnScreenDebugMessage(-1 , DeltaTime , FColor::Red , HealthValue.ToString());
	}

	NumberFormatting.MaximumFractionalDigits = 0;
	const FText FPSValue = FText::Format(LOCTEXT("Key_FPS","FPS : {0}"),FText::AsNumber(1.f / DeltaTime,&NumberFormatting));
	GEngine->AddOnScreenDebugMessage(-1 , DeltaTime , FColor::MakeRedToGreenColorFromScalar(static_cast<float>(FMath::GetMappedRangeValueClamped(FVector2D(0.f , 60.f) , FVector2D(0.f , 1.f) , 1 / DeltaTime))) , FPSValue.ToString());
}

#undef LOCTEXT_NAMESPACE

