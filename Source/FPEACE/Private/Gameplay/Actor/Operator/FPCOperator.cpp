// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPCOperator.h"
#include "EnhancedInputComponent.h"
#include "Components/FPCOperatorAnimationManagerComponent.h"
#include "Components/FPCOperatorCameraManagerComponent.h"
#include "Components/FPCOperatorMovementComponent.h"
#include "Components/FPCOperatorWeaponManagerComponent.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/Actor/FPCGameplayPlayerController.h"
#include "Gameplay/ExtendedClasses/FPCGameInstance.h"
#include "Gameplay/ExtendedClasses/Components/FPCCameraComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCCapsuleComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCSpringArmComponent.h"


// Sets default values
AFPCOperator::AFPCOperator()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FPCCapsuleComp = Cast<UFPCCapsuleComponent>(GetCapsuleComponent());
	FPCMovementComp = Cast<UFPCOperatorMovementComponent>(ACharacter::GetMovementComponent());

	// Create Components

	if (!FPSArmsMeshComp)
	{
		FPSArmsMeshComp = CreateDefaultSubobject<UFPCSkeletalMeshComponent>(TEXT("FPSArms"));
		FPSArmsMeshComp->SetupAttachment(MainBodyMeshComp);
		FPSArmsMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FPSArmsMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
		FPSArmsMeshComp->SetCastShadow(false);
		FPSArmsMeshComp->FPC_SetOnlyOwnerSee(true);
	}

	if (!FPSLowerBodyMeshComp)
	{
		FPSLowerBodyMeshComp = CreateDefaultSubobject<UFPCSkeletalMeshComponent>(TEXT("FPSLowerBody"));
		FPSLowerBodyMeshComp->SetupAttachment(MainBodyMeshComp);
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
		FPCCameraManagerComp = CreateDefaultSubobject<UFPCOperatorCameraManagerComponent>(TEXT("FPC Camera Manager"));

	if (!FPCCharacterWeaponManagerComp)
		FPCCharacterWeaponManagerComp = CreateDefaultSubobject<UFPCOperatorWeaponManagerComponent>(TEXT("FPC Weapon Manager"));

	if (!FPCCharacterAnimationManagerComp)
		FPCCharacterAnimationManagerComp = CreateDefaultSubobject<UFPCOperatorAnimationManagerComponent>(TEXT("FPC Animation Manager"));
}

TWeakObjectPtr<UFPCCharacterData> AFPCOperator::GetCharacterData()
{
	// Get the Character Data asset reference
	if (FPCCharacterData == nullptr)
		if (UFPCGameInstance* FPCGameInstance = UFPCGameInstance::GetInstance(this))
			FPCCharacterData = FPCGameInstance->CharacterData.LoadSynchronous();

	return FPCCharacterData;
}

TWeakObjectPtr<UFPCOperatorMovementComponent> AFPCOperator::GetCharacterMovementComponent() const
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

TWeakObjectPtr<UFPCOperatorMovementComponent> AFPCOperator::GetFPCMovementComp() const
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

TWeakObjectPtr<UFPCOperatorWeaponManagerComponent> AFPCOperator::GetFPCCharacterWeaponManager() const
{
	return FPCCharacterWeaponManagerComp;
}

TWeakObjectPtr<UFPCOperatorCameraManagerComponent> AFPCOperator::GetFPCCharacterCameraManager() const
{
	return FPCCameraManagerComp;
}

TWeakObjectPtr<UFPCOperatorAnimationManagerComponent> AFPCOperator::GetFPCCharacterAnimationManager() const
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
		EInputComp->BindActionInstanceLambda(RunAction.LoadSynchronous(), ETriggerEvent::Started, [this](const FInputActionInstance&)
		{
			FPCMovementComp->ToggleRunSprint();
		});
		EInputComp->BindActionInstanceLambda(CrouchAction.LoadSynchronous(), ETriggerEvent::Started, [this](const FInputActionInstance&)
		{
			FPCMovementComp->ToggleCrouch();
		});
		EInputComp->BindActionInstanceLambda(JumpAction.LoadSynchronous(), ETriggerEvent::Started, [this](const FInputActionInstance&)
		{
			Jump();
		});
		EInputComp->BindActionInstanceLambda(JumpAction.LoadSynchronous(), ETriggerEvent::Completed, [this](const FInputActionInstance&)
		{
			StopJumping();
		});
		EInputComp->BindActionInstanceLambda(ADSAction.LoadSynchronous(), ETriggerEvent::Started, [this](const FInputActionInstance&)
		{
			FPCCharacterWeaponManagerComp->SwitchADSState(true);
		});
		EInputComp->BindActionInstanceLambda(ADSAction.LoadSynchronous(), ETriggerEvent::Completed, [this](const FInputActionInstance&)
		{
			FPCCharacterWeaponManagerComp->SwitchADSState(false);
		});
		EInputComp->BindActionInstanceLambda(FireAction.LoadSynchronous(), ETriggerEvent::Started, [this](const FInputActionInstance&)
		{
			FPCCharacterWeaponManagerComp->ToggleWeaponUse(true);
		});
		EInputComp->BindActionInstanceLambda(FireAction.LoadSynchronous(), ETriggerEvent::Completed, [this](const FInputActionInstance&)
		{
			FPCCharacterWeaponManagerComp->ToggleWeaponUse(false);
		});
		EInputComp->BindActionInstanceLambda(WeaponCycleUpAction.LoadSynchronous(), ETriggerEvent::Started, [this](const FInputActionInstance&)
		{
			FPCCharacterWeaponManagerComp->CycleWeaponInSatchel();
		});
		EInputComp->BindActionInstanceLambda(WeaponCycleDownAction.LoadSynchronous(), ETriggerEvent::Started, [this](const FInputActionInstance&)
		{
			FPCCharacterWeaponManagerComp->CycleWeaponInSatchel(false);
		});
		EInputComp->BindActionInstanceLambda(CameraSwitchAction.LoadSynchronous(), ETriggerEvent::Completed, [this](const FInputActionInstance&)
		{
			FPCCameraManagerComp->ToggleCameraMode();
		});
		EInputComp->BindActionInstanceLambda(ReloadAction.LoadSynchronous(), ETriggerEvent::Started, [this](const FInputActionInstance&)
		{
			FPCCharacterWeaponManagerComp->TryCurrentGunReload();
		});
	}
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
