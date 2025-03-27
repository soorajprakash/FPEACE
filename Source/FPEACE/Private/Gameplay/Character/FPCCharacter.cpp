// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCCharacter.h"
#include "EnhancedInputComponent.h"
#include "FPCCameraComponent.h"
#include "FPCCapsuleComponent.h"
#include "FPCCharacterAnimationManagerComponent.h"
#include "FPCCharacterCameraManagerComponent.h"
#include "FPCCharacterMovementComponent.h"
#include "FPCCharacterWeaponManagerComponent.h"
#include "FPCPlayerController.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/FPCGameInstance.h"
#include "Gameplay/FPCSkeletalMeshComponent.h"
#include "Gameplay/FPCSpringArmComponent.h"

// Sets default values
AFPCCharacter::AFPCCharacter(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer.SetDefaultSubobjectClass<UFPCSkeletalMeshComponent>(MeshComponentName).SetDefaultSubobjectClass<UFPCCharacterMovementComponent>(CharacterMovementComponentName).
	                  SetDefaultSubobjectClass<UFPCCapsuleComponent>(CapsuleComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Get references to the Extensions 
	TPSBodyMeshComp = Cast<UFPCSkeletalMeshComponent>(GetMesh());
	TPSBodyMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	TPSBodyMeshComp->SetCastHiddenShadow(true);

	FPCCapsuleComp = Cast<UFPCCapsuleComponent>(GetCapsuleComponent());
	FPCMovementComp = Cast<UFPCCharacterMovementComponent>(ACharacter::GetMovementComponent());

	// Create Components

	if (!FPSArmsMeshComp)
	{
		FPSArmsMeshComp = CreateDefaultSubobject<UFPCSkeletalMeshComponent>(TEXT("FPSArms"));
		FPSArmsMeshComp->SetupAttachment(TPSBodyMeshComp);
		FPSArmsMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FPSArmsMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
		FPSArmsMeshComp->SetCastShadow(false);
		FPSArmsMeshComp->SetOnlyOwnerSee(true);
	}

	if (!FPSLowerBodyMeshComp)
	{
		FPSLowerBodyMeshComp = CreateDefaultSubobject<UFPCSkeletalMeshComponent>(TEXT("FPSLowerBody"));
		FPSLowerBodyMeshComp->SetupAttachment(TPSBodyMeshComp);
		FPSLowerBodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FPSLowerBodyMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
		FPSLowerBodyMeshComp->SetCastShadow(false);
		FPSLowerBodyMeshComp->SetOnlyOwnerSee(true);
	}

	if (!FPCSpringArmComp)
	{
		FPCSpringArmComp = CreateDefaultSubobject<UFPCSpringArmComponent>(TEXT("FPCSpringArm"));
		FPCSpringArmComp->SetupAttachment(FPCCapsuleComp);
		FPCSpringArmComp->bInheritRoll = false;
	}

	if (!CharacterCameraComp)
	{
		CharacterCameraComp = CreateDefaultSubobject<UFPCCameraComponent>(TEXT("TPSCamera"));
		CharacterCameraComp->SetupAttachment(FPCSpringArmComp);
	}

	if (!FPCCameraManagerComp)
		FPCCameraManagerComp = CreateDefaultSubobject<UFPCCharacterCameraManagerComponent>(TEXT("FPC Camera Manager"));

	if (!FPCCharacterWeaponManagerComp)
		FPCCharacterWeaponManagerComp = CreateDefaultSubobject<UFPCCharacterWeaponManagerComponent>(TEXT("FPC Weapon Manager"));

	if (!FPCCharacterAnimationManagerComp)
		FPCCharacterAnimationManagerComp = CreateDefaultSubobject<UFPCCharacterAnimationManagerComponent>(TEXT("FPC Animation Manager"));
}

void AFPCCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	// Get a reference to character data from the player controller instance
	GetCharacterData();
}

void AFPCCharacter::PossessedBy(AController* NewController)
{
	FPCPlayerControllerInstance = CastChecked<AFPCPlayerController>(NewController);
	Super::PossessedBy(NewController);
}

// Called to bind functionality to input
void AFPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EInputComp->BindAction(LookAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &AFPCCharacter::LookAround);
		EInputComp->BindAction(MoveAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &AFPCCharacter::MoveAround);
		EInputComp->BindAction(RunAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCCharacter::ToggleRunSprint);
		EInputComp->BindAction(CrouchAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCCharacter::ToggleCrouch);
		EInputComp->BindAction(JumpAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCCharacter::JumpStarted);
		EInputComp->BindAction(JumpAction.LoadSynchronous(), ETriggerEvent::Completed, this, &AFPCCharacter::JumpEnded);
		EInputComp->BindAction(ADSAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCCharacter::ActivateADS);
		EInputComp->BindAction(ADSAction.LoadSynchronous(), ETriggerEvent::Completed, this, &AFPCCharacter::DeactivateADS);
		EInputComp->BindAction(CameraSwitchAction.LoadSynchronous(), ETriggerEvent::Completed, this, &AFPCCharacter::ToggleCameraMode);
	}
}

void AFPCCharacter::AddControllerPitchInput(float Val)
{
	if (FPCPlayerControllerInstance && FPCCharacterData)
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
	if (FPCPlayerControllerInstance && FPCCharacterData)
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

	FPCCameraManagerComp->UpdateCameraState();
}

void AFPCCharacter::MoveAround(const FInputActionValue& InputActionValue)
{
	FVector2D Input = InputActionValue.Get<FVector2D>();
	Input = Input.GetRotated(-GetControlRotation().Yaw); // Rotate the input to face the character's direction
	FVector Input3D = FVector(Input.Y, Input.X, 0);
	AddMovementInput(Input3D);
}

void AFPCCharacter::ToggleRunSprint()
{
	FPCMovementComp->ToggleRunSprint();
}

void AFPCCharacter::ToggleCrouch()
{
	FPCMovementComp->ToggleCrouch();
}

void AFPCCharacter::ToggleCameraMode()
{
	FPCCameraManagerComp->ToggleCameraMode();
}

void AFPCCharacter::ActivateADS()
{
	FPCCharacterWeaponManagerComp->SwitchADSState(true);
	FPCCameraManagerComp->SwitchCameraFOV(true);
}

void AFPCCharacter::DeactivateADS()
{
	FPCCharacterWeaponManagerComp->SwitchADSState(false);
	FPCCameraManagerComp->SwitchCameraFOV(false);
}

void AFPCCharacter::JumpStarted(const FInputActionValue& InputActionValue)
{
	Jump();
}

void AFPCCharacter::JumpEnded(const FInputActionValue& InputActionValue)
{
	StopJumping();
}

UFPCCharacterData* AFPCCharacter::GetCharacterData()
{
	// Get the Character Data asset reference
	if (FPCCharacterData == nullptr)
		if (UFPCGameInstance* FPCGameInstance = UFPCGameInstance::GetInstance(this))
			FPCCharacterData = FPCGameInstance->CharacterData.LoadSynchronous();

	return FPCCharacterData;
}
