// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCCharacterAnimationManagerComponent.h"
#include "FPCCharacter.h"
#include "FPCCharacterWeaponManagerComponent.h"
#include "FPCPlayerController.h"
#include "DataStructures/FCameraModeAnimSelectionStruct.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/FPCSkeletalMeshComponent.h"
#include "Gameplay/AnimInstanceClasses/FPCLayerAnimInstance.h"
#include "Gameplay/AnimInstanceClasses/FPCSkeletalAnimInstance.h"


// Sets default values for this component's properties
UFPCCharacterAnimationManagerComponent::UFPCCharacterAnimationManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UFPCCharacterAnimationManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (OwningCharacter == nullptr)
		OwningCharacter = Cast<AFPCCharacter>(GetOwner());

	if (OwningCharacter)
	{
		TPSBodyMeshComp = OwningCharacter->GetTPSBodyMeshComp();
		FPSBodyMeshComp = OwningCharacter->GetFPSBodyMeshComp();
		FPCCharacterMovementComp = OwningCharacter->GetCharacterMovementComponent();
		FPCWeaponManagerComp = OwningCharacter->GetFPCCharacterWeaponManager();
		FPCCharacterData = OwningCharacter->GetCharacterData();
	}

	FPCWeaponManagerComp->OnNewWeaponEquipped.AddDynamic(this, &UFPCCharacterAnimationManagerComponent::OnEquipNewWeapon);
}

void UFPCCharacterAnimationManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwningCharacter)
	{
		PlayerControllerRef = OwningCharacter->GetFPCPlayerController();
		TPSMeshAnimInstance = Cast<UFPCSkeletalAnimInstance>(TPSBodyMeshComp->GetAnimInstance());
		TPSMeshAnimInstance->isBaseAnimInstance = true;
		FPSMeshAnimInstance = Cast<UFPCSkeletalAnimInstance>(FPSBodyMeshComp->GetAnimInstance());
	}
}


// Called every frame
void UFPCCharacterAnimationManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Calculate Play rate
	CurrentAnimPlayRate = FMath::Clamp(FPCCharacterMovementComp->CharacterAbsoluteSpeed2D / FPCCharacterMovementComp->MaxWalkSpeed, 0.f, 1.f);

	CharacterLookSpineVertical = FRotator::ZeroRotator;
	CharacterLookSpineVertical.Roll = -PlayerControllerRef->GetControlRotation().Pitch / 3;

	UpdateValuesForAnimation();
}

void UFPCCharacterAnimationManagerComponent::LinkCombatAnimClassToCharacter(FName AnimClassNameToLink) const
{
	if (TPSMeshAnimInstance->CurrentLinkedAnimInstance)
	{
		TPSBodyMeshComp->UnlinkAnimClassLayers(TPSMeshAnimInstance->CurrentLinkedAnimInstance->GetClass());
		TPSBodyMeshComp->ResetLinkedAnimInstances();
		TPSMeshAnimInstance->CurrentLinkedAnimInstance = nullptr;
	}

	UClass* TPSCombatAnimClass = GetAnimClassFor(ECameraMode::TPS, AnimClassNameToLink, FString(TEXT("Linking Anim Class To Character"))).LoadSynchronous();
	TPSBodyMeshComp->LinkAnimClassLayers(TPSCombatAnimClass);
	TPSMeshAnimInstance->CurrentLinkedAnimInstance = Cast<UFPCLayerAnimInstance>(TPSBodyMeshComp->GetLinkedAnimLayerInstanceByClass(TPSCombatAnimClass));

	if (FPSMeshAnimInstance->CurrentLinkedAnimInstance)
	{
		FPSBodyMeshComp->UnlinkAnimClassLayers(FPSMeshAnimInstance->CurrentLinkedAnimInstance->GetClass());
		FPSBodyMeshComp->ResetLinkedAnimInstances();
		FPSMeshAnimInstance->CurrentLinkedAnimInstance = nullptr;
	}

	UClass* FPSCombatAnimClass = GetAnimClassFor(ECameraMode::FPS, AnimClassNameToLink, FString(TEXT("Linking Anim Class To Character"))).LoadSynchronous();
	FPSBodyMeshComp->LinkAnimClassLayers(FPSCombatAnimClass);
	FPSMeshAnimInstance->CurrentLinkedAnimInstance = Cast<UFPCLayerAnimInstance>(FPSBodyMeshComp->GetLinkedAnimLayerInstanceByClass(FPSCombatAnimClass));
}

void UFPCCharacterAnimationManagerComponent::UpdateValuesForAnimation()
{
	if (MovementStateChanged)
		PrevMovementState = FPCCharacterMovementComp->IsCharacterMoving;
	MovementStateChanged = FPCCharacterMovementComp->IsCharacterMoving != PrevMovementState;

	if (AccelerationStateChanged)
		PrevAccelerationState = FPCCharacterMovementComp->IsCharacterAccelerating;
	AccelerationStateChanged = FPCCharacterMovementComp->IsCharacterAccelerating != PrevAccelerationState;

	// Update if character has reached close to max speed
	HasCharacterReachedCurrentMaxSpeed = FMath::IsNearlyEqual(FPCCharacterMovementComp->CharacterAbsoluteSpeed2D, FPCCharacterMovementComp->MaxWalkSpeed, 10);

	// Check if Character's Velocity Direction Changed
	if (VelocityDirectionChanged)
		FPCCharacterMovementComp->PrevVelocityDirection = FPCCharacterMovementComp->CurrentVelocityDirection;
	VelocityDirectionChanged = FPCCharacterMovementComp->CurrentVelocityDirection != FPCCharacterMovementComp->PrevVelocityDirection;

	// Check if Character's Acceleration Direction Changed
	if (AccelerationDirectionChanged)
		FPCCharacterMovementComp->PrevAccelerationDirection = FPCCharacterMovementComp->CurrentAccelerationDirection;
	AccelerationDirectionChanged = FPCCharacterMovementComp->CurrentAccelerationDirection != FPCCharacterMovementComp->PrevAccelerationDirection;

	// Check if Character's Current Locomotion State Changed
	if (CurrentLocomotionStateChanged)
		FPCCharacterMovementComp->PrevLocomotionState = FPCCharacterMovementComp->currentLocomotionState;
	CurrentLocomotionStateChanged = FPCCharacterMovementComp->currentLocomotionState != FPCCharacterMovementComp->PrevLocomotionState;

	// Check if Character's Target Locomotion State Changed
	if (TargetLocomotionStateChanged)
		FPCCharacterMovementComp->PrevTargetLocomotionState = FPCCharacterMovementComp->TargetLocomotionState;
	TargetLocomotionStateChanged = FPCCharacterMovementComp->TargetLocomotionState != FPCCharacterMovementComp->PrevTargetLocomotionState;
}

TSoftClassPtr<UFPCLayerAnimInstance> UFPCCharacterAnimationManagerComponent::GetAnimClassFor(ECameraMode TargetCameraMode, FName AnimStateName, const FString& ReasonForGettingThisAnim) const
{
	TSoftClassPtr<UFPCLayerAnimInstance> ReturnValue;
	if (const UDataTable* AnimClassTable = FPCCharacterData->AnimClassTable.Get())
	{
		if (const FCameraModeAnimSelectionStruct* SelectedItem = AnimClassTable->FindRow<FCameraModeAnimSelectionStruct>(AnimStateName, ReasonForGettingThisAnim))
		{
			switch (TargetCameraMode)
			{
			case ECameraMode::FPS:
				ReturnValue = SelectedItem->FPSAnimationClass;
				break;

			case ECameraMode::TPS:
				ReturnValue = SelectedItem->TPSAnimationClass;
				break;
			}
		}
	}


	return ReturnValue;
}

void UFPCCharacterAnimationManagerComponent::OnEquipNewWeapon(AFPCWeapon* SpawnedWeapon)
{
	if (SpawnedWeapon)
		LinkCombatAnimClassToCharacter(SpawnedWeapon->WeaponAnimLayerClassName);
	else
		LinkCombatAnimClassToCharacter(TEXT("Unarmed"));
}
