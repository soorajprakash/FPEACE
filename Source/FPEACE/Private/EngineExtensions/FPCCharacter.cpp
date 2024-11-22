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
#include "KismetAnimationLibrary.h"
#include "DataStructures/FPCCharacterData.h"
#include "Kismet/KismetMathLibrary.h"

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
	PrimaryActorTick.bCanEverTick = true;

	// Get references to the Extensions 
	BaseMeshComp = Cast<UFPCSkeletalMeshComponent>(GetMesh());
	BaseMeshComp->SetVisibility(false);
	BaseMeshComp->SetHiddenInGame(true);
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

void AFPCCharacter::SetCurrentLocomotionState(ELocomotionState newLocomotionState)
{
	currentLocomotionState = newLocomotionState;
	currentLocomotionStateFloat = UKismetMathLibrary::Conv_ByteToDouble(static_cast<uint8>(currentLocomotionState));
}

ELocomotionDirection AFPCCharacter::CalculateLocomotionDirection(const float LocomotionDirectionAngle) const
{
	// First check for deadzones

	//If current direction is forward and direction angle is within deadzone buffer, keep the direction
	if (CurrentLocomotionDirection == ELocomotionDirection::Forward && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, ForwardLimits.X - DeadZone, ForwardLimits.Y + DeadZone))
		return ELocomotionDirection::Forward;
	//If current direction is backward and direction angle is within deadzone buffer, keep the direction
	if (CurrentLocomotionDirection == ELocomotionDirection::Backward && (LocomotionDirectionAngle < BackwardLimits.X + DeadZone || LocomotionDirectionAngle > BackwardLimits.Y - DeadZone))
		return ELocomotionDirection::Backward;
	//If current direction is right and direction angle is within deadzone buffer, keep the direction
	if (CurrentLocomotionDirection == ELocomotionDirection::Right && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, ForwardLimits.Y - DeadZone, BackwardLimits.Y + DeadZone))
		return ELocomotionDirection::Right;
	//If current direction is left and direction angle is within deadzone buffer, keep the direction
	if (CurrentLocomotionDirection == ELocomotionDirection::Left && UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, BackwardLimits.X - DeadZone, ForwardLimits.X + DeadZone))
		return ELocomotionDirection::Left;

	// If the above conditions weren't met, calculate the direction manually

	//Check for Forward range
	if (UKismetMathLibrary::InRange_FloatFloat(LocomotionDirectionAngle, ForwardLimits.X, ForwardLimits.Y))
		return ELocomotionDirection::Forward;

	// Check for Backward range
	if (LocomotionDirectionAngle < BackwardLimits.X || LocomotionDirectionAngle > BackwardLimits.Y)
		return ELocomotionDirection::Backward;

	//Check for right and left. We only need to test if it's positive angle since we've already ruled out forward and backward
	return LocomotionDirectionAngle > 0 ? ELocomotionDirection::Right : ELocomotionDirection::Left;
}

// Called when the game starts or when spawned
void AFPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get references to the Extensions 
	FPCMovementComp = Cast<UFPCCharacterMovementComponent>(GetMovementComponent());

	// Get the Character Data asset reference
	FPCCharacterData = UFPCGameInstance::GetInstance(this)->CharacterData;

	// Set initial Character values
	SetLocomotionStateSettings(ELocomotionState::Walking);

	// Store required Character data
	if (FPCCharacterData)
	{
		ForwardLimits = FPCCharacterData->CharacterDirectionLimits.ForwardLimits;
		BackwardLimits = FPCCharacterData->CharacterDirectionLimits.BackwardLimits;
		DeadZone = FPCCharacterData->CharacterDirectionLimits.DirectionalDeadzone;
	}
}

void AFPCCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Calculate Velocity
	CharacterVelocity = FPCMovementComp->Velocity;
	CharacterAbsoluteSpeed = UKismetMathLibrary::VSizeXY(CharacterVelocity);

	CharacterVelocity2D = FVector(CharacterVelocity.X, CharacterVelocity.Y, 0);
	CharacterAbsoluteSpeed2D = UKismetMathLibrary::VSizeXY(CharacterVelocity2D);

	//Calculate the direction angle
	DirectionAngle = UKismetAnimationLibrary::CalculateDirection(CharacterVelocity2D, GetActorRotation());
	CurrentLocomotionDirection = CalculateLocomotionDirection(DirectionAngle);

	// Update transition rule values
	IsCharacterMoving = CharacterVelocity2D.Length() > 0.01f;
}

void AFPCCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	BaseMeshAnimInstance = CastChecked<UFPCAnimInstance>(BaseMeshComp->GetAnimInstance());
	BaseMeshAnimInstance->isBaseAnimInstance = true;
	FPCPlayerControllerInstance = CastChecked<AFPCPlayerController>(NewController);

	// Component Settings
	if (FPCPlayerControllerInstance)
	{
		// Bind to Camera Mode switch callback
		FPCPlayerControllerInstance->OnCameraModeChanged.AddDynamic(this, &AFPCCharacter::SetCameraMode);

		BaseMeshComp->LinkAnimClassLayers(BaseMeshAnimInstance->GetAnimClassFor(ECameraMode::TPS,TEXT("Unarmed"), FString(TEXT("Just for testing"))).LoadSynchronous());
		TPSBodyMeshComp->LinkAnimClassLayers(BaseMeshAnimInstance->GetAnimClassFor(ECameraMode::TPS,TEXT("Unarmed"), FString(TEXT("Just for testing"))).LoadSynchronous());
		FPSBodyMeshComp->LinkAnimClassLayers(BaseMeshAnimInstance->GetAnimClassFor(ECameraMode::TPS,TEXT("Unarmed"), FString(TEXT("Just for testing"))).LoadSynchronous());
		FPSArmsMeshComp->LinkAnimClassLayers(BaseMeshAnimInstance->GetAnimClassFor(ECameraMode::FPS,TEXT("Unarmed"), FString(TEXT("Just for testing"))).LoadSynchronous());
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
		EInputComp->BindAction(RunAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCCharacter::ToggleWalkRun);
		EInputComp->BindAction(SprintAction.LoadSynchronous(), ETriggerEvent::Completed, this, &AFPCCharacter::ToggleSprint);
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

void AFPCCharacter::ToggleWalkRun()
{
	if (currentLocomotionState != ELocomotionState::Running)
		SetCurrentLocomotionState(ELocomotionState::Running);
	else
		SetCurrentLocomotionState(ELocomotionState::Walking);

	SetLocomotionStateSettings(currentLocomotionState);
}

void AFPCCharacter::ToggleSprint()
{
	if (currentLocomotionState != ELocomotionState::Sprinting)
		SetCurrentLocomotionState(ELocomotionState::Sprinting);
	else
		SetCurrentLocomotionState(ELocomotionState::Running);

	SetLocomotionStateSettings(currentLocomotionState);
}

void AFPCCharacter::SetLocomotionStateSettings(ELocomotionState newLocomotionState) const
{
	switch (newLocomotionState)
	{
	case ELocomotionState::Walking:
		{
			FLocomotionStateSetting WalkingSettings = FPCCharacterData->LocomotionStateSettings[ELocomotionState::Walking];

			FPCMovementComp->MaxWalkSpeed = WalkingSettings.MaxWalkSpeed;
			FPCMovementComp->MaxAcceleration = WalkingSettings.MaxAcceleration;
			FPCMovementComp->BrakingDecelerationWalking = WalkingSettings.BrakingDeceleration;
			FPCMovementComp->BrakingFrictionFactor = WalkingSettings.BrakingFrictionFactor;
			FPCMovementComp->BrakingFriction = WalkingSettings.BrakingFriction;
			break;
		}

	case ELocomotionState::Running:
		{
			FLocomotionStateSetting RunningSettings = FPCCharacterData->LocomotionStateSettings[ELocomotionState::Running];

			FPCMovementComp->MaxWalkSpeed = RunningSettings.MaxWalkSpeed;
			FPCMovementComp->MaxAcceleration = RunningSettings.MaxAcceleration;
			FPCMovementComp->BrakingDecelerationWalking = RunningSettings.BrakingDeceleration;
			FPCMovementComp->BrakingFrictionFactor = RunningSettings.BrakingFrictionFactor;
			FPCMovementComp->BrakingFriction = RunningSettings.BrakingFriction;
			break;
		}

	case ELocomotionState::Sprinting:
		{
			FLocomotionStateSetting SprintSettings = FPCCharacterData->LocomotionStateSettings[ELocomotionState::Sprinting];

			FPCMovementComp->MaxWalkSpeed = SprintSettings.MaxWalkSpeed;
			FPCMovementComp->MaxAcceleration = SprintSettings.MaxAcceleration;
			FPCMovementComp->BrakingDecelerationWalking = SprintSettings.BrakingDeceleration;
			FPCMovementComp->BrakingFrictionFactor = SprintSettings.BrakingFrictionFactor;
			FPCMovementComp->BrakingFriction = SprintSettings.BrakingFriction;
			break;
		}
	default: break;
	}
}
