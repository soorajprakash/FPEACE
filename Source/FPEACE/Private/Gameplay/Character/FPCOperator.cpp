// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCOperator.h"
#include "EnhancedInputComponent.h"
#include "FPCCameraComponent.h"
#include "FPCCapsuleComponent.h"
#include "FPCCharacterAnimationManagerComponent.h"
#include "FPCCharacterCameraManagerComponent.h"
#include "FPCCharacterMovementComponent.h"
#include "FPCCharacterWeaponManagerComponent.h"
#include "FPCGameplayPlayerController.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/FPCGameInstance.h"
#include "Gameplay/FPCSkeletalMeshComponent.h"
#include "Gameplay/FPCSpringArmComponent.h"


// Sets default values
AFPCOperator::AFPCOperator()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
		FPSArmsMeshComp->FPC_SetOnlyOwnerSee(true);
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

TWeakObjectPtr<UFPCCharacterData> AFPCOperator::GetCharacterData()
{
	// Get the Character Data asset reference
	if (FPCCharacterData == nullptr)
		if (UFPCGameInstance* FPCGameInstance = UFPCGameInstance::GetInstance(this))
			FPCCharacterData = FPCGameInstance->CharacterData.LoadSynchronous();

	return FPCCharacterData;
}

TWeakObjectPtr<UFPCCharacterMovementComponent> AFPCOperator::GetCharacterMovementComponent() const
{
	return FPCMovementComp;
}

TWeakObjectPtr<UFPCSkeletalMeshComponent> AFPCOperator::GetFPSArmsMeshComp() const
{
	return FPSArmsMeshComp;
}

TWeakObjectPtr<UFPCSkeletalMeshComponent> AFPCOperator::GetFPSLowerBodyMeshComp() const
{
	return FPSLowerBodyMeshComp;
}

TWeakObjectPtr<UFPCCapsuleComponent> AFPCOperator::GetFPCCapsuleComp() const
{
	return FPCCapsuleComp;
}

TWeakObjectPtr<UFPCCharacterMovementComponent> AFPCOperator::GetFPCMovementComp() const
{
	return FPCMovementComp;
}

TWeakObjectPtr<UFPCCameraComponent> AFPCOperator::GetCharacterCameraComp() const
{
	return CharacterCameraComp;
}

TWeakObjectPtr<UFPCSpringArmComponent> AFPCOperator::GetFPCSpringArmComp() const
{
	return FPCSpringArmComp;
}

TWeakObjectPtr<UFPCCharacterWeaponManagerComponent> AFPCOperator::GetFPCCharacterWeaponManager() const
{
	return FPCCharacterWeaponManagerComp;
}

TWeakObjectPtr<UFPCCharacterCameraManagerComponent> AFPCOperator::GetFPCCharacterCameraManager() const
{
	return FPCCameraManagerComp;
}

TWeakObjectPtr<UFPCCharacterAnimationManagerComponent> AFPCOperator::GetFPCCharacterAnimationManager() const
{
	return FPCCharacterAnimationManagerComp;
}

// Called when the game starts or when spawned
void AFPCOperator::BeginPlay()
{
	Super::BeginPlay();
}

void AFPCOperator::AddControllerPitchInput(float Val)
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

void AFPCOperator::AddControllerYawInput(float Val)
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

void AFPCOperator::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	// Get a reference to character data from the player controller instance
	GetCharacterData();
}

// Called every frame
void AFPCOperator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFPCOperator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Gameplay Inputs
		EInputComp->BindAction(LookAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &AFPCOperator::LookAround);
		EInputComp->BindAction(MoveAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &AFPCOperator::MoveAround);
		EInputComp->BindAction(RunAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCOperator::ToggleRunSprint);
		// EInputComp->BindActionInstanceLambda(RunAction.LoadSynchronous(), ETriggerEvent::Started, this, [this] { FPCMovementComp->ToggleRunSprint(); });
		EInputComp->BindAction(CrouchAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCOperator::ToggleCrouch);
		EInputComp->BindAction(JumpAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCOperator::JumpStarted);
		EInputComp->BindAction(JumpAction.LoadSynchronous(), ETriggerEvent::Completed, this, &AFPCOperator::JumpEnded);
		EInputComp->BindAction(ADSAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCOperator::ActivateADS);
		EInputComp->BindAction(ADSAction.LoadSynchronous(), ETriggerEvent::Completed, this, &AFPCOperator::DeactivateADS);
		EInputComp->BindAction(FireAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCOperator::StartUsingWeapon);
		EInputComp->BindAction(FireAction.LoadSynchronous(), ETriggerEvent::Completed, this, &AFPCOperator::StopUsingWeapon);
		EInputComp->BindAction(WeaponCycleUpAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCOperator::CycleWeaponUp);
		EInputComp->BindAction(WeaponCycleDownAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCOperator::CycleWeaponDown);
		EInputComp->BindAction(CameraSwitchAction.LoadSynchronous(), ETriggerEvent::Completed, this, &AFPCOperator::ToggleCameraMode);
		EInputComp->BindAction(ReloadAction.LoadSynchronous(), ETriggerEvent::Started, this, &AFPCOperator::TriggerWeaponReload);
	}
}

void AFPCOperator::JumpStarted(const FInputActionValue& InputActionValue)
{
	Jump();
}

void AFPCOperator::JumpEnded(const FInputActionValue& InputActionValue)
{
	StopJumping();
}

void AFPCOperator::LookAround(const FInputActionValue& InputActionValue)
{
	FVector2D input = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(input.X);
	AddControllerPitchInput(input.Y);

	FPCCameraManagerComp->UpdateCameraState();
}

void AFPCOperator::MoveAround(const FInputActionValue& InputActionValue)
{
	FVector2D Input = InputActionValue.Get<FVector2D>();
	Input = Input.GetRotated(-GetControlRotation().Yaw); // Rotate the input to face the character's direction
	FVector Input3D = FVector(Input.Y, Input.X, 0);
	AddMovementInput(Input3D);
}

void AFPCOperator::ToggleRunSprint()
{
}

void AFPCOperator::ToggleCrouch()
{
	FPCMovementComp->ToggleCrouch();
}

void AFPCOperator::ToggleCameraMode()
{
	FPCCameraManagerComp->ToggleCameraMode();
}

void AFPCOperator::ActivateADS()
{
	FPCCharacterWeaponManagerComp->SwitchADSState(true);
}

void AFPCOperator::DeactivateADS()
{
	FPCCharacterWeaponManagerComp->SwitchADSState(false);
}

void AFPCOperator::TriggerWeaponReload(const FInputActionValue& InputActionValue)
{
	FPCCharacterWeaponManagerComp->TryCurrentGunReload();
}

void AFPCOperator::CycleWeaponUp()
{
	FPCCharacterWeaponManagerComp->CycleWeaponInSatchel();
}

void AFPCOperator::CycleWeaponDown()
{
	FPCCharacterWeaponManagerComp->CycleWeaponInSatchel(false);
}

void AFPCOperator::StartUsingWeapon()
{
	FPCCharacterWeaponManagerComp->ToggleWeaponUse(true);
}

void AFPCOperator::StopUsingWeapon()
{
	FPCCharacterWeaponManagerComp->ToggleWeaponUse(false);
}
