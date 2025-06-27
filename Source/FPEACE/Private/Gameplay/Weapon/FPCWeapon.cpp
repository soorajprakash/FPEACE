// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCWeapon.h"
#include "CommonEnums.h"
#include "Gameplay/Character/FPCCharacterCameraManagerComponent.h"
#include "Gameplay/Character/FPCOperator.h"

AFPCWeapon::AFPCWeapon(): UsedInCameraMode(ECameraMode::TPS), bIsWeaponInUse(false)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFPCWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	WeaponMeshComps = GatherWeaponMeshComps();
}

void AFPCWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

TArray<TObjectPtr<UMeshComponent>> AFPCWeapon::GatherWeaponMeshComps()
{
	/*
	 * Use this to gather all mesh components that make up the weapon.
	 * To be overridden by derived classes.
	 */
	return TArray<TObjectPtr<UMeshComponent>>();
}

void AFPCWeapon::OnCameraModeChanged(ECameraMode NewCameraMode)
{
	bCameraModeMatchesWeapon = UsedInCameraMode == NewCameraMode;
}

void AFPCWeapon::UseWeapon()
{
	/*
	 * Do the logic of using the weapon here.
	 * Guns can fire bullets, mêlée weapons can be swiped, fists can be used to punch, etc.
	 */
}

void AFPCWeapon::SetupWeapon(const ECameraMode TargetCameraMode, USceneComponent* AttachCharacterMesh)
{
	//Get References from Owning Character
	if (OwningOperator == nullptr)
		OwningOperator = Cast<AFPCOperator>(AttachCharacterMesh->GetOwner());

	if (OwningOperator.IsValid())
	{
		SetOwner(OwningOperator.Get());

		if (!OwningCharacterMovementComponent.IsValid())
			OwningCharacterMovementComponent = OwningOperator->GetCharacterMovementComponent();

		if (!OwningCharacterCameraManager.IsValid())
			OwningCharacterCameraManager = OwningOperator->GetFPCCharacterCameraManager();

		if (!OwningCharacterWeaponManager.IsValid())
			OwningCharacterWeaponManager = OwningOperator->GetFPCCharacterWeaponManager();

		if (!OwningCharacterAnimationManager.IsValid())
			OwningCharacterAnimationManager = OwningOperator->GetFPCCharacterAnimationManager();
	}

	UsedInCameraMode = TargetCameraMode;
	bCameraModeMatchesWeapon = UsedInCameraMode == OwningCharacterCameraManager->GetCurrentCameraMode();

	// Attach this weapon instance to the character's mesh
	AttachToComponent(AttachCharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("SOCKET_Weapon"));

	for (TObjectPtr WeaponComp : WeaponMeshComps)
	{
		if (WeaponComp)
		{
			WeaponComp->SetOnlyOwnerSee(UsedInCameraMode == ECameraMode::FPS);
			WeaponComp->SetCastShadow(UsedInCameraMode == ECameraMode::TPS);
			WeaponComp->SetCastHiddenShadow(UsedInCameraMode == ECameraMode::TPS);
			WeaponComp->FirstPersonPrimitiveType = TargetCameraMode == ECameraMode::FPS ? EFirstPersonPrimitiveType::FirstPerson : EFirstPersonPrimitiveType::None;
		}
	}

	// Register events
	if (OwningCharacterCameraManager.IsValid())
	{
		if (UFPCCharacterCameraManagerComponent* CManager = OwningCharacterCameraManager.Get())
		{
			CManager->OnCameraModeChanged.RemoveAll(this);
			CManager->OnCameraModeChanged.AddDynamic(this, &AFPCWeapon::OnCameraModeChanged);
		}
	}
}

void AFPCWeapon::ToggleVisibilityToOwner(bool bIsVisible)
{
	for (TObjectPtr WeaponComp : WeaponMeshComps)
		if (WeaponComp)
			WeaponComp->SetOwnerNoSee(!bIsVisible);
}
