// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCCharacter.h"

#include "CommonEnums.h"
#include "FPCCapsuleComponent.h"
#include "FPCCharacterMovementComponent.h"
#include "FPCSkeletalMeshComponent.h"
#include "FPCCameraComponent.h"
#include "FPCSpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "FPCAnimInstance.h"
#include "FPCGameInstance.h"
#include "FPCPlayerController.h"
#include "DataStructures/FPCCharacterData.h"

void AFPCCharacter::SetCameraMode(ECameraMode NewCameraMode)
{
	if (FPCPlayerControllerInstance)
	{
		// Based on the camera mode, toggle the skeletal meshes
		// Set up the spring Arm and Camera components according to current camera mode
		switch (NewCameraMode)
		{
		case ECameraMode::FPS:
			TPSBodyMeshComp->SetHiddenInGame(true);
			FPSBodyMeshComp->SetHiddenInGame(false);
			FPSArmsMeshComp->SetHiddenInGame(false);

			FPCCameraComp->AttachToComponent(FPSArmsMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("SOCKET_Camera"));
			break;

		case ECameraMode::TPS:
			TPSBodyMeshComp->SetHiddenInGame(false);
			FPSBodyMeshComp->SetHiddenInGame(true);
			FPSArmsMeshComp->SetHiddenInGame(true);

			FPCCameraComp->AttachToComponent(FPCSpringArmComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			break;
		}
	}
}

// Sets default values
AFPCCharacter::AFPCCharacter(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer.SetDefaultSubobjectClass<UFPCSkeletalMeshComponent>(MeshComponentName).SetDefaultSubobjectClass<UFPCCharacterMovementComponent>(CharacterMovementComponentName).
	                  SetDefaultSubobjectClass<UFPCCapsuleComponent>(CapsuleComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Get references to the Extensions 
	BaseMeshComp = Cast<UFPCSkeletalMeshComponent>(GetMesh());
	BaseMeshComp->SetVisibility(false);
	BaseMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	FPCCapsuleComp = Cast<UFPCCapsuleComponent>(GetCapsuleComponent());

	// Create Components
	if (!TPSBodyMeshComp)
	{
		TPSBodyMeshComp = CreateDefaultSubobject<UFPCSkeletalMeshComponent>(TEXT("TPSBody"));
		TPSBodyMeshComp->SetupAttachment(BaseMeshComp);
	}

	if (!FPSBodyMeshComp)
	{
		FPSBodyMeshComp = CreateDefaultSubobject<UFPCSkeletalMeshComponent>(TEXT("FPSBody"));
		FPSBodyMeshComp->SetupAttachment(BaseMeshComp);
	}

	if (!FPSArmsMeshComp)
	{
		FPSArmsMeshComp = CreateDefaultSubobject<UFPCSkeletalMeshComponent>(TEXT("FPSArms"));
		FPSArmsMeshComp->SetupAttachment(FPSBodyMeshComp);
	}

	if (!FPCSpringArmComp)
	{
		FPCSpringArmComp = CreateDefaultSubobject<UFPCSpringArmComponent>(TEXT("FPCSpringArm"));
		FPCSpringArmComp->SetupAttachment(FPCCapsuleComp);
		FPCSpringArmComp->bUsePawnControlRotation = true;
	}

	if (!FPCCameraComp)
	{
		FPCCameraComp = CreateDefaultSubobject<UFPCCameraComponent>(TEXT("FPCCamera"));
		FPCCameraComp->SetupAttachment(FPCSpringArmComp);
	}
}

// Called when the game starts or when spawned
void AFPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get references to the Extensions 
	FPCMovementComp = Cast<UFPCCharacterMovementComponent>(GetMovementComponent());
	BaseMeshAnimInstance = CastChecked<UFPCAnimInstance>(BaseMeshComp->GetAnimInstance());

	//Set initial Character values
	SetLocomotionStateSettings(Walking);
}

void AFPCCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FPCPlayerControllerInstance = CastChecked<AFPCPlayerController>(NewController);

	// Component Settings
	if (FPCPlayerControllerInstance)
	{
		// Bind to Camera Mode switch callback
		FPCPlayerControllerInstance->OnCameraModeChanged.AddDynamic(this, &AFPCCharacter::SetCameraMode);

		BaseMeshComp->LinkAnimClassLayers(UFPCAnimInstance::GetAnimClassFor(this, ECameraMode::TPS,TEXT("Unarmed"), FString(TEXT("Just for testing"))).LoadSynchronous());
		FPSArmsMeshComp->LinkAnimClassLayers(UFPCAnimInstance::GetAnimClassFor(this, ECameraMode::FPS,TEXT("Unarmed"), FString(TEXT("Just for testing"))).LoadSynchronous());
	}
}

// Called to bind functionality to input
void AFPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EInputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EInputComp->BindAction(LookAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &AFPCCharacter::LookAround);
		EInputComp->BindAction(MoveAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &AFPCCharacter::MoveAround);
		EInputComp->BindAction(SprintAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCCharacter::ToggleWalkRunSprint);
		EInputComp->BindAction(SprintAction.LoadSynchronous(), ETriggerEvent::Completed, this, &AFPCCharacter::ToggleWalkRunSprint);
	}
}

void AFPCCharacter::LookAround(const FInputActionValue& InputActionValue)
{
	FVector2D input = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(input.X);
	AddControllerPitchInput(input.Y);
}

void AFPCCharacter::MoveAround(const FInputActionValue& InputActionValue)
{
	FVector2D input = InputActionValue.Get<FVector2D>();
	input = input.GetRotated(-GetControlRotation().Yaw); // Rotate the input to face the character's direction
	AddMovementInput(FVector(input.Y, input.X, 0));
}

void AFPCCharacter::ToggleWalkRunSprint()
{
	if (currentLocomotionState == Walking) currentLocomotionState = Running;
	else currentLocomotionState = Walking;

	SetLocomotionStateSettings(currentLocomotionState);

	// Update Anim Blueprint
	if (BaseMeshAnimInstance) BaseMeshAnimInstance->currentLocomotionState = currentLocomotionState;
}

void AFPCCharacter::SetLocomotionStateSettings(TEnumAsByte<ELocomotionState> newLocomotionState) const
{
	// Get the Character Data asset reference
	UFPCCharacterData* FPCCharacterData = UFPCGameInstance::GetInstance(this)->CharacterData.Get();

	switch (newLocomotionState)
	{
	case Walking:
		{
			FLocomotionStateSetting WalkingSettings = FPCCharacterData->LocomotionStateSettings[Walking];

			FPCMovementComp->MaxWalkSpeed = WalkingSettings.MaxWalkSpeed;
			FPCMovementComp->MaxAcceleration = WalkingSettings.MaxAcceleration;
			FPCMovementComp->BrakingDecelerationWalking = WalkingSettings.BrakingDeceleration;
			FPCMovementComp->BrakingFrictionFactor = WalkingSettings.BrakingFrictionFactor;
			FPCMovementComp->BrakingFriction = WalkingSettings.BrakingFriction;
			break;
		}

	case Running:
		{
			FLocomotionStateSetting RunningSettings = FPCCharacterData->LocomotionStateSettings[Running];

			FPCMovementComp->MaxWalkSpeed = RunningSettings.MaxWalkSpeed;
			FPCMovementComp->MaxAcceleration = RunningSettings.MaxAcceleration;
			FPCMovementComp->BrakingDecelerationWalking = RunningSettings.BrakingDeceleration;
			FPCMovementComp->BrakingFrictionFactor = RunningSettings.BrakingFrictionFactor;
			FPCMovementComp->BrakingFriction = RunningSettings.BrakingFriction;
			break;
		}

	case TacSprinting:
		//TODO Add Sprint settings
		break;
	default: break;
	}
}
