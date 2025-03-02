// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCCharacterWeaponManagerComponent.h"
#include "CommonEnums.h"
#include "FPCCharacter.h"
#include "FPCCharacterCameraManagerComponent.h"
#include "FPCCharacterMovementComponent.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/FPCSkeletalMeshComponent.h"
#include "Gameplay/Weapon/FPCWeapon.h"

// Sets default values for this component's properties
UFPCCharacterWeaponManagerComponent::UFPCCharacterWeaponManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UFPCCharacterWeaponManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (OwningCharacter == nullptr)
		OwningCharacter = Cast<AFPCCharacter>(GetOwner());

	if (OwningCharacter)
	{
		OwningCharacterMovementComp = OwningCharacter->GetCharacterMovementComponent();
		FPCCharacterData = OwningCharacter->GetCharacterData();
		TPSBodyMeshComp = OwningCharacter->GetTPSBodyMeshComp();
		FPSBodyMeshComp = OwningCharacter->GetFPSBodyMeshComp();
		FPCCameraManagerComp = OwningCharacter->GetFPCCharacterCameraManager();
	}

	if (FPCCameraManagerComp)
		FPCCameraManagerComp->OnCameraModeChanged.AddDynamic(this, &UFPCCharacterWeaponManagerComponent::CharacterCameraModeChanged);

	if (OwningCharacterMovementComp)
		OwningCharacterMovementComp->OnCurrentLocomotionStateChanged.AddDynamic(this, &UFPCCharacterWeaponManagerComponent::CharacterCurrentLocomotionStateChanged);
}

void UFPCCharacterWeaponManagerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateCurrentWeaponHandIKOffset(DeltaTime);
}

void UFPCCharacterWeaponManagerComponent::UpdateWeaponVisibility(const bool IsInTPSCameraMode) const
{
	if (CurrentTPSWeaponRef)
		CurrentTPSWeaponRef->ToggleVisibilityToOwner(IsInTPSCameraMode);

	if (CurrentFPSWeaponRef)
		CurrentFPSWeaponRef->SetActorHiddenInGame(IsInTPSCameraMode);
}

void UFPCCharacterWeaponManagerComponent::SwitchADSState(bool UseADS)
{
	bIsCharacterInADSState = UseADS;

	SetTargetWeaponHandIKOffset();
}

void UFPCCharacterWeaponManagerComponent::PickUpAndEquipWeapon(const TSoftClassPtr<AFPCWeapon>& WeaponBP)
{
	// Destroy current weapon instance and disarm the character
	if (CurrentFPSWeaponRef)
	{
		CurrentFPSWeaponRef->Destroy();
		CurrentFPSWeaponRef = nullptr;
	}

	if (CurrentTPSWeaponRef)
	{
		CurrentTPSWeaponRef->Destroy();
		CurrentTPSWeaponRef = nullptr;
	}

	if (WeaponBP != nullptr)
	{
		UClass* WeaponBPRef = WeaponBP.LoadSynchronous();
		// Spawn one weapon instance for each FPS and TPS mesh
		CurrentFPSWeaponRef = Cast<AFPCWeapon>(GetWorld()->SpawnActor(WeaponBPRef));
		CurrentTPSWeaponRef = Cast<AFPCWeapon>(GetWorld()->SpawnActor(WeaponBPRef));

		if (CurrentFPSWeaponRef && CurrentTPSWeaponRef)
		{
			CurrentFPSWeaponRef->SetupWeapon(ECameraMode::FPS, FPSBodyMeshComp);
			CurrentTPSWeaponRef->SetupWeapon(ECameraMode::TPS, TPSBodyMeshComp);
		}

		CurrentWeaponAnimSettings = CurrentFPSWeaponRef->GetAnimSettings();
	}
	else
	{
		CurrentWeaponAnimSettings = FWeaponAnimSettings();
	}

	bIsCharacterArmed = WeaponBP != nullptr;

	UpdateWeaponVisibility(FPCCameraManagerComp->IsInTPSCameraMode);

	SetTargetWeaponHandIKOffset();

	// Can use the name from either FPS or TPS weapon ref since they will be the same
	OnNewWeaponEquipped.Broadcast(CurrentFPSWeaponRef);
}

void UFPCCharacterWeaponManagerComponent::CharacterCameraModeChanged(ECameraMode NewCameraMode)
{
	UpdateWeaponVisibility(NewCameraMode == ECameraMode::TPS);
}

void UFPCCharacterWeaponManagerComponent::CharacterCurrentLocomotionStateChanged(ELocomotionState NewLocomotionState)
{
	SetTargetWeaponHandIKOffset();
}

void UFPCCharacterWeaponManagerComponent::SetTargetWeaponHandIKOffset()
{
	if (!CurrentFPSWeaponRef || !CurrentTPSWeaponRef)
		return;

	if (bIsCharacterInADSState)
	{
		TargetWeaponHandIKLocationOffset = CurrentWeaponAnimSettings.ADSLocomotionStateOffsets[OwningCharacterMovementComp->GetCurrentLocomotionState()].GetLocation();
		TargetWeaponHandIKRotationOffset = CurrentWeaponAnimSettings.ADSLocomotionStateOffsets[OwningCharacterMovementComp->GetCurrentLocomotionState()].Rotator();
	}
	else
	{
		TargetWeaponHandIKLocationOffset = CurrentWeaponAnimSettings.DefaultLocomotionStateOffsets[OwningCharacterMovementComp->GetCurrentLocomotionState()].GetLocation();
		TargetWeaponHandIKRotationOffset = CurrentWeaponAnimSettings.DefaultLocomotionStateOffsets[OwningCharacterMovementComp->GetCurrentLocomotionState()].Rotator();
	}
}

void UFPCCharacterWeaponManagerComponent::UpdateCurrentWeaponHandIKOffset(const float DeltaTime)
{
	CurrentWeaponHandIKLocationOffset = FMath::VInterpTo(CurrentWeaponHandIKLocationOffset, TargetWeaponHandIKLocationOffset, DeltaTime, FPCCharacterData->IKBoneOffsetInterpSpeed);
	CurrentWeaponHandIKRotationOffset = FMath::RInterpTo(CurrentWeaponHandIKRotationOffset, TargetWeaponHandIKRotationOffset, DeltaTime, FPCCharacterData->IKBoneOffsetInterpSpeed);
}
