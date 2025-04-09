// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCGun.h"
#include "Gameplay/FPCSkeletalMeshComponent.h"
#include "Gameplay/FPCStaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Gameplay/Character/FPCCharacterCameraManagerComponent.h"
#include "FPCBullet.h"
#include "ObjectPoolSubsystem.h"
#include "Gameplay/Character/FPCCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AFPCGun::AFPCGun()
{
	if (!ReceiverMeshComp)
	{
		ReceiverMeshComp = CreateDefaultSubobject<UFPCSkeletalMeshComponent>(TEXT("ReceiverMeshComp"));
		RootComponent = ReceiverMeshComp;
	}

	if (!MagazineMeshComp)
	{
		MagazineMeshComp = CreateDefaultSubobject<UFPCStaticMeshComponent>(TEXT("MagazineMeshComp"));
		MagazineMeshComp->SetupAttachment(ReceiverMeshComp, FName(TEXT("SOCKET_Magazine")));
	}

	if (!MuzzleMeshComp)
	{
		MuzzleMeshComp = CreateDefaultSubobject<UFPCStaticMeshComponent>(TEXT("MuzzleMeshComp"));
		MuzzleMeshComp->SetupAttachment(ReceiverMeshComp, FName(TEXT("SOCKET_Muzzle")));
	}

	if (!OpticMeshComp)
	{
		OpticMeshComp = CreateDefaultSubobject<UFPCStaticMeshComponent>(TEXT("OpticMeshComp"));
		OpticMeshComp->SetupAttachment(ReceiverMeshComp, FName(TEXT("SOCKET_Scope")));
	}

	if (!IronSightMeshComp)
	{
		IronSightMeshComp = CreateDefaultSubobject<UFPCStaticMeshComponent>(TEXT("IronsightMeshComp"));
		IronSightMeshComp->SetupAttachment(ReceiverMeshComp, FName(TEXT("SOCKET_Default")));
	}
}

void AFPCGun::InitiateMuzzleFlash()
{
	if (!MuzzleMeshComp)
		return;

	FTransform EmitterSocketTransform = MuzzleMeshComp->GetSocketTransform(TEXT("SOCKET_Emitter"));

	// Particle effect to be spawned at the emitter socket
	UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlashEffect, EmitterSocketTransform.GetLocation(),
	                                                                                  EmitterSocketTransform.GetRotation().Rotator());
	SpawnedEffect->GetOwner()->SetOwner(OwningCharacter);
	SpawnedEffect->SetOnlyOwnerSee(UsedInCameraMode == ECameraMode::FPS);
	SpawnedEffect->SetOwnerNoSee(UsedInCameraMode != OwningCharacterCameraManager->GetCurrentCameraMode());
}

void AFPCGun::UseWeapon()
{
	Super::UseWeapon();

	if (bIsWeaponReadyToBeUsed)
	{
		switch (GunSettings.FireMode)
		{
		case SingleShot:
			{
				Fire();
				bIsWeaponInCoolDown = true;
				GunCoolDownHandle.Invalidate();
				FTimerDelegate CooldownTimerDelegate = FTimerDelegate::CreateLambda([this]()
				{
					bIsWeaponInCoolDown = false;
				});

				GetWorld()->GetTimerManager().SetTimer(GunCoolDownHandle, CooldownTimerDelegate, 1 / GunSettings.FireRate, false);
				break;
			}

		case BurstFire:
			{
				bIsWeaponInCoolDown = true;
				RemainingShotsInBurst = GunSettings.FireRate;
				BurstModeFire();
				break;
			}

		case Automatic:
			{
				Fire();
				bIsWeaponInCoolDown = true;
				GunCoolDownHandle.Invalidate();
				FTimerDelegate CooldownTimerDelegate = FTimerDelegate::CreateLambda([this]()
				{
					bIsWeaponInCoolDown = false;
				});

				GetWorld()->GetTimerManager().SetTimer(GunCoolDownHandle, CooldownTimerDelegate, 1 / GunSettings.FireRate, false);
				break;
			}
		default: break;
		}
	}
	else
	{
		// Check when the weapon can fire/again

		switch (GunSettings.FireMode)
		{
		case SingleShot:
			{
				if (!bIsWeaponInCoolDown && bWasTriggerLiftedAfterLastFire)
					bIsWeaponReadyToBeUsed = true;
				break;
			}

		case BurstFire:
			{
				if (!bIsWeaponInCoolDown && bWasTriggerLiftedAfterLastFire)
					bIsWeaponReadyToBeUsed = true;
				break;
			}

		case Automatic:
			{
				if (!bIsWeaponInCoolDown)
					bIsWeaponReadyToBeUsed = true;
				break;
			}

		default: break;
		}
	}
}

TArray<TObjectPtr<UMeshComponent>> AFPCGun::GatherWeaponMeshComps()
{
	return TArray<TObjectPtr<UMeshComponent>>{ReceiverMeshComp, MagazineMeshComp, MuzzleMeshComp, OpticMeshComp, IronSightMeshComp};
}

void AFPCGun::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Get required Socket Transforms
	AimSocketActorSpaceTransform = OpticMeshComp->GetSocketTransform(TEXT("SOCKET_Aim"), RTS_Actor);
	EmitterSocketActorSpaceTransform = MuzzleMeshComp->GetSocketTransform(TEXT("SOCKET_Emitter"), RTS_Actor);

	// Set up the gun to have no collision
	// TODO : Collision is probably required. Change this to suit the game's needs
	SetActorEnableCollision(false);
}

void AFPCGun::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsWeaponInUse)
	{
		UseWeapon();
		bWasTriggerLiftedAfterLastFire = false;
	}
	else
		bWasTriggerLiftedAfterLastFire = true;
}

void AFPCGun::SetupWeapon(const ECameraMode TargetCameraMode, USceneComponent* AttachCharacterMesh)
{
	Super::SetupWeapon(TargetCameraMode, AttachCharacterMesh);

	// Pool the bullets
	if (BulletClass && OwningCharacter->WorldObjectPool)
	{
		FPooledActorSettings BulletPoolSettings;
		BulletPoolSettings.bCanExpand = false;
		BulletPoolSettings.InitialSpawnCount = GunSettings.MagCapacity;
		OwningCharacter->WorldObjectPool->AddActorType(BulletClass, BulletPoolSettings);
	}
}

void AFPCGun::Fire()
{
	FTransform BulletSpawnTransform;
	FTransform EmitterSocketTransform = MuzzleMeshComp->GetSocketTransform(TEXT("SOCKET_Emitter"));

	// For FPS camera mode, we need to shoot the bullet from the aim socket's height but at the emitter's distance
	// The bullet gets propelled in the direction from the muzzle location to the camera's intended target
	if (UsedInCameraMode == ECameraMode::FPS)
	{
		const FTransform AimSocketTransform = OpticMeshComp->GetSocketTransform(TEXT("SOCKET_Aim"));
		const FVector RelativeEmitterLocation = AimSocketTransform.InverseTransformPosition(EmitterSocketTransform.GetLocation());
		const FVector FinalSpawnLocation = AimSocketTransform.TransformPosition(FVector(0, RelativeEmitterLocation.Y, 0));

		BulletSpawnTransform = AimSocketTransform;
		BulletSpawnTransform.SetLocation(FinalSpawnLocation);
	}
	else if (UsedInCameraMode == ECameraMode::TPS)
	{
		BulletSpawnTransform = EmitterSocketTransform;
	}

	if (UsedInCameraMode == OwningCharacterCameraManager->GetCurrentCameraMode())
	{
		FRotator FireDirection = FRotationMatrix::MakeFromY((OwningCharacterCameraManager->GetCurrentCameraLookAtHit() - BulletSpawnTransform.GetLocation())).Rotator();
		BulletSpawnTransform.SetRotation(FireDirection.Quaternion());
		if (AFPCBullet* NewBullet = AcquireBullet())
			NewBullet->PropelBullet(*OwningCharacter, *this, BulletSpawnTransform, GunSettings.BulletVelocity);
	}

	OnWeaponSuccessfullyUsed.Broadcast();
	bIsWeaponReadyToBeUsed = false;
}

void AFPCGun::BurstModeFire()
{
	if (RemainingShotsInBurst > 0)
	{
		Fire();
		RemainingShotsInBurst--;

		if (RemainingShotsInBurst > 0)
			GetWorld()->GetTimerManager().SetTimer(GunContinuosFireHandle, this, &AFPCGun::BurstModeFire, GunSettings.BurstFireInterval, false);
		else
			BurstModeFire();
	}
	else
	{
		GunCoolDownHandle.Invalidate();
		GetWorld()->GetTimerManager().SetTimer(GunCoolDownHandle, [this]()
		{
			UE_LOG(LogTemp, Warning, TEXT("Burst Fired"));
			bIsWeaponInCoolDown = false;
		}, GunSettings.FireCoolDownInterval, false);
	}
}

AFPCBullet* AFPCGun::AcquireBullet() const
{
	AActor* BulletActorFromPool;
	OwningCharacter->WorldObjectPool->Pull(BulletClass, BulletActorFromPool);
	if (AFPCBullet* NewBullet = Cast<AFPCBullet>(BulletActorFromPool))
		return NewBullet;

	return nullptr;
}
