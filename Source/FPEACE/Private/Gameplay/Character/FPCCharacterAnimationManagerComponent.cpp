// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCCharacterAnimationManagerComponent.h"
#include "FPCCharacter.h"
#include "FPCCharacterWeaponManagerComponent.h"
#include "FPCGameplayPlayerController.h"
#include "DataStructures/FCameraModeAnimSelectionStruct.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/FPCSkeletalMeshComponent.h"
#include "Gameplay/AnimInstanceClasses/FPCLayerAnimInstance.h"
#include "Gameplay/AnimInstanceClasses/FPCSkeletalAnimInstance.h"
#include "Gameplay/Weapon/FPCGun.h"


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
		FPCCharacterMovementComp = OwningCharacter->GetCharacterMovementComponent();
		FPCWeaponManagerComp = OwningCharacter->GetFPCCharacterWeaponManager();
		FPCCharacterData = OwningCharacter->GetCharacterData();

		TPSBodyMeshComp = OwningCharacter->GetTPSBodyMeshComp();
		FPSBodyMeshComp = OwningCharacter->GetFPSArmsMeshComp();
		TPSMeshAnimInstance = Cast<UFPCSkeletalAnimInstance>(TPSBodyMeshComp->GetAnimInstance());
		if (TPSMeshAnimInstance)
			TPSMeshAnimInstance->isBaseAnimInstance = true;
		FPSMeshAnimInstance = Cast<UFPCSkeletalAnimInstance>(FPSBodyMeshComp->GetAnimInstance());
	}

	FPCWeaponManagerComp->OnNewWeaponEquipped.AddDynamic(this, &UFPCCharacterAnimationManagerComponent::OnEquipNewWeapon);
}

void UFPCCharacterAnimationManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwningCharacter)
		PlayerControllerRef = OwningCharacter->GetFPCPlayerController();
}

// Called every frame
void UFPCCharacterAnimationManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Calculate Play rate
	CurrentAnimPlayRate = FMath::Clamp(FPCCharacterMovementComp->GetCharacterAbsoluteSpeed2D() / FPCCharacterMovementComp->MaxWalkSpeed, 0.f, 1.f);

	CharacterLookSpineVertical = FRotator::ZeroRotator;
	CharacterLookSpineVertical.Roll = -PlayerControllerRef->GetControlRotation().Pitch / 3;

	// Update if character has reached close to max speed
	HasCharacterReachedCurrentMaxSpeed = FMath::IsNearlyEqual(FPCCharacterMovementComp->GetCharacterAbsoluteSpeed2D(), FPCCharacterMovementComp->MaxWalkSpeed, 10);
}

void UFPCCharacterAnimationManagerComponent::LinkCombatAnimClassToCharacter(FName AnimClassNameToLink) const
{
	if (TPSMeshAnimInstance->CurrentLinkedAnimInstance)
	{
		TPSBodyMeshComp->UnlinkAnimClassLayers(TPSMeshAnimInstance->CurrentLinkedAnimInstance->GetClass());
		TPSBodyMeshComp->ResetLinkedAnimInstances();
		TPSMeshAnimInstance->CurrentLinkedAnimInstance->MarkAsGarbage();
		TPSMeshAnimInstance->CurrentLinkedAnimInstance = nullptr;
	}

	UClass* TPSCombatAnimClass = GetAnimClassFor(ECameraMode::TPS, AnimClassNameToLink, FString(TEXT("Linking Anim Class To Character"))).LoadSynchronous();
	TPSBodyMeshComp->LinkAnimClassLayers(TPSCombatAnimClass);
	TPSMeshAnimInstance->CurrentLinkedAnimInstance = Cast<UFPCLayerAnimInstance>(TPSBodyMeshComp->GetLinkedAnimLayerInstanceByClass(TPSCombatAnimClass));
	TPSCombatAnimClass = nullptr;

	if (FPSMeshAnimInstance->CurrentLinkedAnimInstance)
	{
		FPSBodyMeshComp->UnlinkAnimClassLayers(FPSMeshAnimInstance->CurrentLinkedAnimInstance->GetClass());
		FPSBodyMeshComp->ResetLinkedAnimInstances();
		FPSMeshAnimInstance->CurrentLinkedAnimInstance->MarkAsGarbage();
		FPSMeshAnimInstance->CurrentLinkedAnimInstance = nullptr;
	}

	UClass* FPSCombatAnimClass = GetAnimClassFor(ECameraMode::FPS, AnimClassNameToLink, FString(TEXT("Linking Anim Class To Character"))).LoadSynchronous();
	FPSBodyMeshComp->LinkAnimClassLayers(FPSCombatAnimClass);
	FPSMeshAnimInstance->CurrentLinkedAnimInstance = Cast<UFPCLayerAnimInstance>(FPSBodyMeshComp->GetLinkedAnimLayerInstanceByClass(FPSCombatAnimClass));
	FPSCombatAnimClass = nullptr;
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

void UFPCCharacterAnimationManagerComponent::OnEquipNewWeapon(AFPCWeapon* SpawnedFPSWeapon, AFPCWeapon* SpawnedTPSWeapon)
{
	// Link the animation class to the character
	if (SpawnedFPSWeapon && SpawnedTPSWeapon)
	{
		LinkCombatAnimClassToCharacter(SpawnedFPSWeapon->WeaponAnimLayerClassName);

		// Subscribe to the weapon's animation related events events
		SpawnedFPSWeapon->OnWeaponSuccessfullyUsed.RemoveAll(this);
		SpawnedFPSWeapon->OnWeaponSuccessfullyUsed.AddDynamic(this, &UFPCCharacterAnimationManagerComponent::OnCurrentFPSWeaponUsed);

		SpawnedTPSWeapon->OnWeaponSuccessfullyUsed.RemoveAll(this);
		SpawnedTPSWeapon->OnWeaponSuccessfullyUsed.AddDynamic(this, &UFPCCharacterAnimationManagerComponent::OnCurrentTPSWeaponUsed);

		if (AFPCGun* SpawnedGun = Cast<AFPCGun>(SpawnedFPSWeapon))
		{
			SpawnedGun->OnReloadStarted.RemoveAll(this);
			SpawnedGun->OnReloadStarted.AddDynamic(this, &UFPCCharacterAnimationManagerComponent::OnGunReloadStart);
		}

		if (AFPCGun* SpawnedGun = Cast<AFPCGun>(SpawnedTPSWeapon))
		{
			SpawnedGun->OnReloadStarted.RemoveAll(this);
			SpawnedGun->OnReloadStarted.AddDynamic(this, &UFPCCharacterAnimationManagerComponent::OnGunReloadStart);
		}
	}
	else
		LinkCombatAnimClassToCharacter(TEXT("Unarmed"));

	// Notify Layer about new weapon
	FPSMeshAnimInstance->CurrentLinkedAnimInstance->OnWeaponEquipped(SpawnedFPSWeapon);
	TPSMeshAnimInstance->CurrentLinkedAnimInstance->OnWeaponEquipped(SpawnedTPSWeapon);
}

void UFPCCharacterAnimationManagerComponent::OnCurrentFPSWeaponUsed()
{
	FPSMeshAnimInstance->CurrentLinkedAnimInstance->OnCurrentWeaponUsed();
}

void UFPCCharacterAnimationManagerComponent::OnCurrentTPSWeaponUsed()
{
	TPSMeshAnimInstance->CurrentLinkedAnimInstance->OnCurrentWeaponUsed();
}

void UFPCCharacterAnimationManagerComponent::OnGunReloadStart(bool bEmptyReload, AFPCGun* ReloadingGun)
{
	if (ReloadingGun->UsedInCameraMode == ECameraMode::FPS)
		FPSMeshAnimInstance->CurrentLinkedAnimInstance->OnCurrentWeaponReloadStart(bEmptyReload);
	else if (ReloadingGun->UsedInCameraMode == ECameraMode::TPS)
		TPSMeshAnimInstance->CurrentLinkedAnimInstance->OnCurrentWeaponReloadStart(bEmptyReload);
}
