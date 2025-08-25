// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPCOperator.h"
#include "EnhancedInputComponent.h"
#include "Components/FPCFullScreenJoystickComponent.h"
#include "Components/FPCOperatorAnimationManagerComponent.h"
#include "Components/FPCOperatorCameraManagerComponent.h"
#include "Components/FPCOperatorMovementComponent.h"
#include "Components/FPCOperatorWeaponManagerComponent.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/Actor/FPCGameplayPlayerController.h"
#include "Gameplay/Common/FPCGameplayTags.h"
#include "Gameplay/ExtendedClasses/FPCGameInstance.h"
#include "Gameplay/ExtendedClasses/FPCPlayerState.h"
#include "Gameplay/ExtendedClasses/Components/FPCAbilitySystemComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCCameraComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCCapsuleComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCSpringArmComponent.h"
#include "Gameplay/GAS/AttribueSets/FPCHealthAttributeSet.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFPCOperator::AFPCOperator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UFPCOperatorMovementComponent>(CharacterMovementComponentName))
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

	if (!FPCFullScreenJoystickComp)
		FPCFullScreenJoystickComp = CreateDefaultSubobject<UFPCFullScreenJoystickComponent>(TEXT("FullScreenJoystickComponent"));
}

void AFPCOperator::OnReceivedDamage(TWeakObjectPtr<AFPCCharacter> From, FName HitBone)
{
	Super::OnReceivedDamage(From, HitBone);

	if (HealthAttributeSet->GetHealth() <= 0)
	{
		FPCMovementComp->DisableMovement();
		FPCAbilitySystemComponent->CancelAllAbilities();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		MainBodyMeshComp->SetCollisionProfileName("Ragdoll");
		MainBodyMeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		MainBodyMeshComp->SetSimulatePhysics(true);
		OnDeath();

		if (FPCCameraManagerComp->GetCurrentCameraMode() == ECameraMode::FPS)
			FPCCameraManagerComp->ToggleCameraMode();
	}
	else
	{
		//Determine the direction of the enemy who dealt the blow
		float Angle = UKismetMathLibrary::FindRelativeLookAtRotation(GetActorTransform(), From->GetActorLocation()).Yaw;

		// Determine the hit reaction animation based on the hit direction
		UAnimMontage* SelectedReactionMontage;
		if (Angle <= 45 && Angle >= -45)
			SelectedReactionMontage = HitReaction_Front;
		else if (Angle > 45 && Angle < 135)
			SelectedReactionMontage = HitReaction_Right;
		else if (Angle > 135 && Angle < -135)
			SelectedReactionMontage = HitReaction_Back;
		else
			SelectedReactionMontage = HitReaction_Left;

		OnHitDamageTaken(SelectedReactionMontage);
	}

	PlayTookMeleeDamageSound();
}

void AFPCOperator::PlayEnemyHitRegisterSound(bool bIsKillShot) const
{
	UGameplayStatics::PlaySound2D(GetWorld(), bIsKillShot ? EnemyKillEffectSound : EnemyHitEffectSound);
}

void AFPCOperator::PlayTookMeleeDamageSound() const
{
	UGameplayStatics::PlaySound2D(GetWorld(), TookMeleeDamageEffectSound);
}

void AFPCOperator::PlayPlayerDeathEffectSound() const
{
	UGameplayStatics::PlaySound2D(GetWorld(), PlayerDeathSoundEffect);
}

void AFPCOperator::PlayDamageGruntSound(bool bIsKillShot) const
{
	UGameplayStatics::PlaySound2D(GetWorld(), DamageTakenGrunt);
}

TWeakObjectPtr<UFPCOperatorData> AFPCOperator::GetOperatorData()
{
	// Get the Character Data asset reference
	if (FPCOperatorData == nullptr)
		if (UFPCGameInstance* FPCGameInstance = UFPCGameInstance::GetInstance(this))
			FPCOperatorData = FPCGameInstance->CharacterData;

	return FPCOperatorData;
}

TWeakObjectPtr<AFPCGameplayPlayerController> AFPCOperator::GetFPCPlayerController() const
{
	return FPCPlayerControllerInstance;
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

TWeakObjectPtr<UFPCFullScreenJoystickComponent> AFPCOperator::GetFPCFullScreenJoystickComp() const
{
	return FPCFullScreenJoystickComp;
}

void AFPCOperator::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	GetOperatorData();
}

void AFPCOperator::BeginPlay()
{
	Super::BeginPlay();

	if (FPCFullScreenJoystickComp)
		FPCFullScreenJoystickComp->SetDragAnywhereEnabled(true);
}

void AFPCOperator::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	FPCPlayerControllerInstance = CastChecked<AFPCGameplayPlayerController>(NewController);
}

// Called to bind functionality to input
void AFPCOperator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Gameplay Inputs
		EInputComp->BindAction(ToggleSprintAction, ETriggerEvent::Started, this, &AFPCOperator::OnSprintAction);
		EInputComp->BindAction(FireAction, ETriggerEvent::Started, this, &AFPCOperator::OnFireActionStarted);
		EInputComp->BindAction(FireAction, ETriggerEvent::Completed, this, &AFPCOperator::OnFireActionEnded);
		EInputComp->BindAction(CrouchAction, ETriggerEvent::Started, this, &AFPCOperator::OnCrouchAction);
		EInputComp->BindAction(JumpAction, ETriggerEvent::Started, this, &AFPCOperator::OnJumpActionStarted);
		EInputComp->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFPCOperator::OnJumpActionEnded);
		EInputComp->BindAction(ADSAction, ETriggerEvent::Started, this, &AFPCOperator::OnADSActionStarted);
		EInputComp->BindAction(ADSAction, ETriggerEvent::Completed, this, &AFPCOperator::OnADSActionEnded);
		EInputComp->BindAction(WeaponCycleUpAction, ETriggerEvent::Started, this, &AFPCOperator::OnWeaponCycleUpAction);
		EInputComp->BindAction(WeaponCycleDownAction, ETriggerEvent::Started, this, &AFPCOperator::OnWeaponCycleDownAction);
		EInputComp->BindAction(CameraSwitchAction, ETriggerEvent::Completed, this, &AFPCOperator::OnCameraSwitchAction);
		EInputComp->BindAction(ReloadAction, ETriggerEvent::Started, this, &AFPCOperator::OnReloadAction);
	}
}

void AFPCOperator::OnDeath_Implementation()
{
	Super::OnDeath_Implementation();

	FPCAbilitySystemComponent->AddLooseGameplayTag(Character_Health_Dead);

	if (AFPCPlayerState* PS = Cast<AFPCPlayerState>(GetPlayerState()))
		PS->StopSurvivalTimer();

	PlayPlayerDeathEffectSound();
}

void AFPCOperator::OnSprintAction()
{
	FPCMovementComp->ToggleRunSprint();
}

void AFPCOperator::OnFireActionStarted()
{
	FPCCharacterWeaponManagerComp->ToggleWeaponUse(true);
}

void AFPCOperator::OnFireActionEnded()
{
	FPCCharacterWeaponManagerComp->ToggleWeaponUse(false);
}

void AFPCOperator::OnCrouchAction()
{
	FPCMovementComp->ToggleCrouch();
}

void AFPCOperator::OnJumpActionStarted()
{
	GetCharacterMovementComponent()->Jump();
}

void AFPCOperator::OnJumpActionEnded()
{
	StopJumping();
}

void AFPCOperator::OnADSActionStarted()
{
	FPCCharacterWeaponManagerComp->SwitchADSState(true);
}

void AFPCOperator::OnADSActionEnded()
{
	FPCCharacterWeaponManagerComp->SwitchADSState(false);
}

void AFPCOperator::OnWeaponCycleUpAction()
{
	FPCCharacterWeaponManagerComp->CycleWeaponInSatchel();
}

void AFPCOperator::OnWeaponCycleDownAction()
{
	FPCCharacterWeaponManagerComp->CycleWeaponInSatchel(false);
}

void AFPCOperator::OnCameraSwitchAction()
{
	FPCCameraManagerComp->ToggleCameraMode();
}

void AFPCOperator::OnReloadAction()
{
	FPCCharacterWeaponManagerComp->TryCurrentGunReload();
}
