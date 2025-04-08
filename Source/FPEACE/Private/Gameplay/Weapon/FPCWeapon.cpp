// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCWeapon.h"
#include "CommonEnums.h"
#include "Gameplay/Character/FPCCharacter.h"
#include "Gameplay/Character/FPCPlayerController.h"

AFPCWeapon::AFPCWeapon(): UsedInCameraMode(ECameraMode::TPS)
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
	if (OwningCharacter == nullptr)
		OwningCharacter = Cast<AFPCCharacter>(AttachCharacterMesh->GetOwner());

	if (OwningCharacter)
	{
		SetOwner(OwningCharacter);

		if (!OwningCharacterMovementComponent)
			OwningCharacterMovementComponent = OwningCharacter->GetCharacterMovementComponent();

		if (!OwningCharacterCameraManager)
			OwningCharacterCameraManager = OwningCharacter->GetFPCCharacterCameraManager();

		if (!OwningCharacterWeaponManager)
			OwningCharacterWeaponManager = OwningCharacter->GetFPCCharacterWeaponManager();

		if (!OwningCharacterAnimationManager)
			OwningCharacterAnimationManager = OwningCharacter->GetFPCCharacterAnimationManager();
	}

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
