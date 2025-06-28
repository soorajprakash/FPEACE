// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCOperatorAnimationManagerComponent.h"
#include "FPCOperatorMovementComponent.h"
#include "FPCOperatorWeaponManagerComponent.h"
#include "DataStructures/FCameraModeAnimSelectionStruct.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/AnimInstanceClasses/Operator/FPCOperatorLayerAnimInstance.h"
#include "Gameplay/AnimInstanceClasses/Operator/FPCOperatorSkeletalAnimInstance.h"
#include "Gameplay/Actor/FPCGameplayPlayerController.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"
#include "Gameplay/Weapon/FPCGun.h"


// Sets default values for this component's properties
UFPCOperatorAnimationManagerComponent::UFPCOperatorAnimationManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UFPCOperatorAnimationManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	FPCOperatorWeaponManagerComp->OnNewWeaponEquipped.AddDynamic(this, &UFPCOperatorAnimationManagerComponent::OnEquipNewWeapon);
}

void UFPCOperatorAnimationManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UFPCOperatorAnimationManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Calculate Play rate
	CurrentAnimPlayRate = FMath::Clamp(FPCOperatorMovementComp->GetCharacterAbsoluteSpeed2D() / FPCOperatorMovementComp->MaxWalkSpeed, 0.f, 1.f);

	CharacterLookSpineVertical = FRotator::ZeroRotator;
	CharacterLookSpineVertical.Roll = -PlayerControllerRef->GetControlRotation().Pitch / 3;

	// Update if character has reached close to max speed
	HasCharacterReachedCurrentMaxSpeed = FMath::IsNearlyEqual(FPCOperatorMovementComp->GetCharacterAbsoluteSpeed2D(), FPCOperatorMovementComp->MaxWalkSpeed, 10);
}

void UFPCOperatorAnimationManagerComponent::LinkCombatAnimClassToCharacter(FName AnimClassNameToLink) const
{
	if (TPSMeshAnimInstance->CurrentLinkedAnimInstance)
	{
		MainBodyMeshComp->UnlinkAnimClassLayers(TPSMeshAnimInstance->CurrentLinkedAnimInstance->GetClass());
		MainBodyMeshComp->ResetLinkedAnimInstances();
		TPSMeshAnimInstance->CurrentLinkedAnimInstance->MarkAsGarbage();
		TPSMeshAnimInstance->CurrentLinkedAnimInstance = nullptr;
	}

	UClass* TPSCombatAnimClass = GetAnimClassFor(ECameraMode::TPS, AnimClassNameToLink, FString(TEXT("Linking Anim Class To Character"))).LoadSynchronous();
	MainBodyMeshComp->LinkAnimClassLayers(TPSCombatAnimClass);
	TPSMeshAnimInstance->CurrentLinkedAnimInstance = Cast<UFPCOperatorLayerAnimInstance>(MainBodyMeshComp->GetLinkedAnimLayerInstanceByClass(TPSCombatAnimClass));
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
	FPSMeshAnimInstance->CurrentLinkedAnimInstance = Cast<UFPCOperatorLayerAnimInstance>(FPSBodyMeshComp->GetLinkedAnimLayerInstanceByClass(FPSCombatAnimClass));
	FPSCombatAnimClass = nullptr;
}

TSoftClassPtr<UFPCOperatorLayerAnimInstance> UFPCOperatorAnimationManagerComponent::GetAnimClassFor(ECameraMode TargetCameraMode, FName AnimStateName, const FString& ReasonForGettingThisAnim) const
{
	TSoftClassPtr<UFPCOperatorLayerAnimInstance> ReturnValue;
	if (const UDataTable* AnimClassTable = FPCOperatorData->AnimClassTable.Get())
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

void UFPCOperatorAnimationManagerComponent::OnEquipNewWeapon(AFPCWeapon* SpawnedFPSWeapon, AFPCWeapon* SpawnedTPSWeapon)
{
	// Link the animation class to the character
	if (SpawnedFPSWeapon && SpawnedTPSWeapon)
	{
		LinkCombatAnimClassToCharacter(SpawnedFPSWeapon->WeaponAnimLayerClassName);

		// Subscribe to the weapon's animation related events events
		SpawnedFPSWeapon->OnWeaponSuccessfullyUsed.RemoveAll(this);
		SpawnedFPSWeapon->OnWeaponSuccessfullyUsed.AddDynamic(this, &UFPCOperatorAnimationManagerComponent::OnCurrentFPSWeaponUsed);

		SpawnedTPSWeapon->OnWeaponSuccessfullyUsed.RemoveAll(this);
		SpawnedTPSWeapon->OnWeaponSuccessfullyUsed.AddDynamic(this, &UFPCOperatorAnimationManagerComponent::OnCurrentTPSWeaponUsed);

		if (AFPCGun* SpawnedGun = Cast<AFPCGun>(SpawnedFPSWeapon))
		{
			SpawnedGun->OnReloadStarted.RemoveAll(this);
			SpawnedGun->OnReloadStarted.AddDynamic(this, &UFPCOperatorAnimationManagerComponent::OnGunReloadStart);
		}

		if (AFPCGun* SpawnedGun = Cast<AFPCGun>(SpawnedTPSWeapon))
		{
			SpawnedGun->OnReloadStarted.RemoveAll(this);
			SpawnedGun->OnReloadStarted.AddDynamic(this, &UFPCOperatorAnimationManagerComponent::OnGunReloadStart);
		}
	}
	else
		LinkCombatAnimClassToCharacter(TEXT("Unarmed"));

	// Notify Layer about new weapon
	FPSMeshAnimInstance->CurrentLinkedAnimInstance->OnWeaponEquipped(SpawnedFPSWeapon);
	TPSMeshAnimInstance->CurrentLinkedAnimInstance->OnWeaponEquipped(SpawnedTPSWeapon);
}

void UFPCOperatorAnimationManagerComponent::OnCurrentFPSWeaponUsed()
{
	FPSMeshAnimInstance->CurrentLinkedAnimInstance->OnCurrentWeaponUsed();
}

void UFPCOperatorAnimationManagerComponent::OnCurrentTPSWeaponUsed()
{
	TPSMeshAnimInstance->CurrentLinkedAnimInstance->OnCurrentWeaponUsed();
}

void UFPCOperatorAnimationManagerComponent::OnGunReloadStart(bool bEmptyReload, AFPCGun* ReloadingGun)
{
	if (ReloadingGun->UsedInCameraMode == ECameraMode::FPS)
		FPSMeshAnimInstance->CurrentLinkedAnimInstance->OnCurrentWeaponReloadStart(bEmptyReload);
	else if (ReloadingGun->UsedInCameraMode == ECameraMode::TPS)
		TPSMeshAnimInstance->CurrentLinkedAnimInstance->OnCurrentWeaponReloadStart(bEmptyReload);
}
