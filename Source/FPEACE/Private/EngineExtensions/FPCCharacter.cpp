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
			TPSBodyMeshComp->SetVisibility(false);
			FPSBodyMeshComp->SetHiddenInGame(false);
			FPSBodyMeshComp->SetVisibility(true);

			FPCSpringArmComp->AttachToComponent(FPSBodyMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("SOCKET_Camera"));
			FPCSpringArmComp->TargetArmLength = GetCharacterData()->CameraModeSettings[ECameraMode::FPS].MaxSpringArmLength;
			break;

		case ECameraMode::TPS:

			TPSBodyMeshComp->SetHiddenInGame(false);
			TPSBodyMeshComp->SetVisibility(true);
			FPSBodyMeshComp->SetHiddenInGame(true);
			FPSBodyMeshComp->SetVisibility(false);

			FPCSpringArmComp->AttachToComponent(FPCCapsuleComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			FPCSpringArmComp->SetArmLengthFromPitch(GetCharacterData()->CameraModeSettings[ECameraMode::TPS].PitchToArmLengthCurve, FPCPlayerControllerInstance->GetControlRotation());
			break;
		}

		// Switch the camera settings defined in the character's data asset
		SetCameraSettings(NewCameraMode);
	}
}

void AFPCCharacter::SetCameraSettings(ECameraMode CurrentCameraMode)
{
	if (GetCharacterData())
	{
		const FCharacterCameraModeSettings& TargetCameraModeSettings = CurrentCameraMode == ECameraMode::FPS
			                                                               ? FPCCharacterData->CameraModeSettings[ECameraMode::FPS]
			                                                               : FPCCharacterData->CameraModeSettings[ECameraMode::TPS];

		FPCSpringArmComp->SetRelativeLocation(TargetCameraModeSettings.SpringArmTramsformOffset);
		FPCSpringArmComp->SocketOffset = TargetCameraModeSettings.SpringArmSocketOffset;
		FPCSpringArmComp->TargetOffset = TargetCameraModeSettings.SpringArmTargetOffset;
		FPCSpringArmComp->bUsePawnControlRotation = TargetCameraModeSettings.bUsePawnControlRotation;
		FPCSpringArmComp->bEnableCameraLag = TargetCameraModeSettings.bEnableCameraLag;
		FPCSpringArmComp->CameraLagSpeed = TargetCameraModeSettings.CameraLagSpeed;
		FPCSpringArmComp->CameraLagMaxDistance = TargetCameraModeSettings.CameraLagMaxDistance;
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

	if (!FPCSpringArmComp)
	{
		FPCSpringArmComp = CreateDefaultSubobject<UFPCSpringArmComponent>(TEXT("FPCSpringArm"));
		FPCSpringArmComp->SetupAttachment(FPCCapsuleComp);
		FPCSpringArmComp->bInheritRoll = false;
	}

	if (!FPCCameraComp)
	{
		FPCCameraComp = CreateDefaultSubobject<UFPCCameraComponent>(TEXT("FPCCamera"));
		FPCCameraComp->SetupAttachment(FPCSpringArmComp);
	}
}

void AFPCCharacter::SetCurrentLocomotionState(ELocomotionState newLocomotionState)
{
	if (currentLocomotionState == newLocomotionState)
		return;

	currentLocomotionState = newLocomotionState;
	OnLocomotionStateChanged.Broadcast(currentLocomotionState);
}

void AFPCCharacter::SetTargetLocomotionState(ELocomotionState newLocomotionState)
{
	if (TargetLocomotionState == newLocomotionState)
		return;

	TargetLocomotionState = newLocomotionState;
}

void AFPCCharacter::SetCurrentLocomotionStateWithSettings(ELocomotionState newLocomotionState)
{
	if (currentLocomotionState == newLocomotionState)
		return;

	SetCurrentLocomotionState(newLocomotionState);
	SetLocomotionStateSettings(currentLocomotionState);
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

void AFPCCharacter::SetLocomotionDirection(ELocomotionDirection newLocomotionDirection)
{
	CurrentLocomotionDirection = newLocomotionDirection;

	// If the direction is not forward then switch to walking
	if (CurrentLocomotionDirection != ELocomotionDirection::Forward)
		SetCurrentLocomotionStateWithSettings(ELocomotionState::Running);
}

// Called when the game starts or when spawned
void AFPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get references to the Extensions 
	FPCMovementComp = Cast<UFPCCharacterMovementComponent>(GetMovementComponent());

	// Set initial Character values
	SetTargetLocomotionState(ELocomotionState::Running);
	SetCurrentLocomotionStateWithSettings(ELocomotionState::Running);

	// Store required Character data
	ForwardLimits = GetCharacterData()->CharacterDirectionLimits.ForwardLimits;
	BackwardLimits = GetCharacterData()->CharacterDirectionLimits.BackwardLimits;
	DeadZone = GetCharacterData()->CharacterDirectionLimits.DirectionalDeadzone;
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
	DirectionAngle = UKismetAnimationLibrary::CalculateDirection(FPCMovementComp->GetLastInputVector(), GetActorRotation());
	UE_LOG(LogTemp, Warning, TEXT("Direction: %f"), DirectionAngle);
	SetLocomotionDirection(CalculateLocomotionDirection(DirectionAngle));

	// Update transition rule values
	UpdateAnimationTransitionRuleValues();

	DynamicLocomotionStateUpdate();
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
		FPSBodyMeshComp->LinkAnimClassLayers(BaseMeshAnimInstance->GetAnimClassFor(ECameraMode::FPS,TEXT("Unarmed"), FString(TEXT("Just for testing"))).LoadSynchronous());
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
		EInputComp->BindAction(RunAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCCharacter::ToggleRunSprint);
	}
}

void AFPCCharacter::AddControllerPitchInput(float Val)
{
	if (FPCPlayerControllerInstance && GetCharacterData())
	{
		FRotator ControlRotation = FPCPlayerControllerInstance->GetControlRotation();
		ControlRotation.Pitch -= Val; // Subtracting instead of adding since we are normailizing the values in the next step.
		ControlRotation.Pitch = FRotator::NormalizeAxis(ControlRotation.Pitch); // Normalizing flips the direction.

		// Clamp the pitch between the limits defined in character data asset
		ControlRotation.Pitch = FMath::Clamp(ControlRotation.Pitch, -FPCCharacterData->ControllerRotationPitchClamp, FPCCharacterData->ControllerRotationPitchClamp);

		// Set the updated control rotation
		FPCPlayerControllerInstance->SetControlRotation(ControlRotation);
	}
}

void AFPCCharacter::AddControllerYawInput(float Val)
{
	if (FPCPlayerControllerInstance && GetCharacterData())
	{
		FRotator ControlRotation = FPCPlayerControllerInstance->GetControlRotation();
		ControlRotation.Yaw += Val;
		ControlRotation.Pitch = FRotator::NormalizeAxis(ControlRotation.Pitch); // Normalizing flips the direction.

		// Set the updated control rotation
		FPCPlayerControllerInstance->SetControlRotation(ControlRotation);
	}
}

void AFPCCharacter::LookAround(const FInputActionValue& InputActionValue)
{
	FVector2D input = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(input.X);
	AddControllerPitchInput(input.Y);

	CharacterLookSpineVertical.Roll = -Controller->GetControlRotation().Pitch;

	if (FPCPlayerControllerInstance->GetCameraMode() == ECameraMode::TPS)
		FPCSpringArmComp->SetArmLengthFromPitch(GetCharacterData()->CameraModeSettings[ECameraMode::TPS].PitchToArmLengthCurve, FPCPlayerControllerInstance->GetControlRotation());
}

void AFPCCharacter::MoveAround(const FInputActionValue& InputActionValue)
{
	FVector2D input = InputActionValue.Get<FVector2D>();
	input = input.GetRotated(-GetControlRotation().Yaw); // Rotate the input to face the character's direction
	AddMovementInput(FVector(input.Y, input.X, 0));
}

void AFPCCharacter::ToggleRunSprint()
{
	if (TargetLocomotionState != ELocomotionState::Running)
		SetTargetLocomotionState(ELocomotionState::Running);
	else
		SetTargetLocomotionState(ELocomotionState::Sprinting);
}

void AFPCCharacter::ToggleWalking()
{
	SetTargetLocomotionState(ELocomotionState::Walking);
}

void AFPCCharacter::SetLocomotionStateSettings(ELocomotionState newLocomotionState)
{
	CurrentMaxLocomotionSpeed = GetCharacterData()->LocomotionStateSettings[currentLocomotionState].MaxWalkSpeed;
	currentLocomotionStateFloat = UKismetMathLibrary::Conv_ByteToDouble(static_cast<uint8>(currentLocomotionState));

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

void AFPCCharacter::UpdateAnimationTransitionRuleValues()
{
	// Update if the character is moving
	if (IsCharacterMoving && CharacterVelocity2D.Length() < 10.f)
	{
		IsCharacterMoving = false;
		SetTargetLocomotionState(ELocomotionState::Running);
	}
	else if (!IsCharacterMoving && CharacterVelocity2D.Length() > 1.f)
		IsCharacterMoving = true;
}

void AFPCCharacter::DynamicLocomotionStateUpdate()
{
	SetCurrentLocomotionStateWithSettings(TargetLocomotionState);
}

UFPCCharacterData* AFPCCharacter::GetCharacterData()
{
	// Get the Character Data asset reference
	if (FPCCharacterData == nullptr)
		FPCCharacterData = UFPCGameInstance::GetInstance(this)->CharacterData;

	return FPCCharacterData;
}
