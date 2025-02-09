// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCCharacterWeaponManagerComponent.h"
#include "CommonEnums.h"
#include "FPCCharacter.h"
#include "FPCCharacterCameraManagerComponent.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/FPCSkeletalMeshComponent.h"
#include "Gameplay/Weapon/FPCWeapon.h"

// Sets default values for this component's properties
UFPCCharacterWeaponManagerComponent::UFPCCharacterWeaponManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UFPCCharacterWeaponManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (OwningCharacter == nullptr)
		OwningCharacter = Cast<AFPCCharacter>(GetOwner());

	if (OwningCharacter)
	{
		FPCCharacterData = OwningCharacter->GetCharacterData();
		TPSBodyMeshComp = OwningCharacter->GetTPSBodyMeshComp();
		FPSBodyMeshComp = OwningCharacter->GetFPSBodyMeshComp();
		FPCCameraManagerComp = OwningCharacter->GetFPCCharacterCameraManager();
	}

	if (FPCCameraManagerComp)
		FPCCameraManagerComp->OnCameraModeChanged.AddDynamic(this,&UFPCCharacterWeaponManagerComponent::CharacterCameraModeChanged);
}

void UFPCCharacterWeaponManagerComponent::UpdateWeaponVisibility(const bool IsInTPSCameraMode) const
{
	if (CurrentTPSWeaponRef)
		CurrentTPSWeaponRef->ToggleVisibilityToOwner(IsInTPSCameraMode);

	if (CurrentFPSWeaponRef)
		CurrentFPSWeaponRef->SetActorHiddenInGame(IsInTPSCameraMode);
}

void UFPCCharacterWeaponManagerComponent::PickUpAndEquipWeapon(const TSubclassOf<AFPCWeapon>& WeaponBP)
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

	if (WeaponBP.Get() != nullptr)
	{
		// Spawn one weapon instance for each FPS and TPS mesh
		CurrentFPSWeaponRef = Cast<AFPCWeapon>(GetWorld()->SpawnActor(WeaponBP));
		CurrentTPSWeaponRef = Cast<AFPCWeapon>(GetWorld()->SpawnActor(WeaponBP));

		if (CurrentFPSWeaponRef && CurrentTPSWeaponRef)
		{
			CurrentFPSWeaponRef->SetupWeapon(ECameraMode::FPS, FPSBodyMeshComp);
			CurrentTPSWeaponRef->SetupWeapon(ECameraMode::TPS, TPSBodyMeshComp);
		}
	}

	bIsCharacterArmed = WeaponBP != nullptr;

	UpdateWeaponVisibility(FPCCameraManagerComp->IsInTPSCameraMode);

	// Can use the name from either FPS or TPS weapon ref since they will be the same
	OnNewWeaponEquipped.Broadcast(CurrentFPSWeaponRef);
}

void UFPCCharacterWeaponManagerComponent::CharacterCameraModeChanged(ECameraMode NewCameraMode)
{
	UpdateWeaponVisibility(NewCameraMode==ECameraMode::TPS);
}
