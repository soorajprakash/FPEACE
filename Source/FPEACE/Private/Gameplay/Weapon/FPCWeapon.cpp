// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCWeapon.h"
#include "CommonEnums.h"

AFPCWeapon::AFPCWeapon(): UsedInCameraMode(ECameraMode::TPS)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFPCWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	WeaponMeshComps = GatherWeaponMeshComps();
}

TArray<TObjectPtr<UMeshComponent>> AFPCWeapon::GatherWeaponMeshComps()
{
	/*
	 * Use this to gather all mesh components that make up the weapon.
	 * To be overridden by derived classes.
	 */
	return TArray<TObjectPtr<UMeshComponent>>();
}

void AFPCWeapon::SetupWeapon(ECameraMode TargetCameraMode, USceneComponent* AttachCharacterMesh)
{
	SetOwner(AttachCharacterMesh->GetOwner());
	UsedInCameraMode = TargetCameraMode;

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
}

void AFPCWeapon::ToggleVisibilityToOwner(bool bIsVisible)
{
	for (TObjectPtr WeaponComp : WeaponMeshComps)
		if (WeaponComp)
			WeaponComp->SetOwnerNoSee(!bIsVisible);
}
