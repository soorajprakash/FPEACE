// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCGun.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "FPCBullet.h"
#include "ObjectPoolSubsystem.h"
#include "RecoilHelper.h"
#include "Gameplay/Actor/Operator/FPCOperator.h"
#include "Gameplay/Actor/Operator/Components/FPCCharacterCameraManagerComponent.h"
#include "Gameplay/Actor/Operator/Components/FPCCharacterWeaponManagerComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCStaticMeshComponent.h"

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
	SpawnedEffect->GetOwner()->SetOwner(OwningOperator.Get());
	SpawnedEffect->SetOnlyOwnerSee(UsedInCameraMode == ECameraMode::FPS);
	SpawnedEffect->SetOwnerNoSee(UsedInCameraMode != OwningCharacterCameraManager->GetCurrentCameraMode());
}

void AFPCGun::UseWeapon()
{
	Super::UseWeapon();

	if (RemainingBulletsInMag < 1 || bIsReloading)
		return;

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
					GunRecoilHelper->RecoilStop();
				});

				GetWorld()->GetTimerManager().SetTimer(GunCoolDownHandle, CooldownTimerDelegate, 1 / GunSettings.FireRate, false);

				// Start Recoil
				GunRecoilHelper->RecoilStart();

				break;
			}

		case BurstFire:
			{
				bIsWeaponInCoolDown = true;
				RemainingShotsInBurst = GunSettings.FireRate;
				BurstModeFire();
				// Start Recoil
				GunRecoilHelper->RecoilStart();
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

				// Start Recoil
				GunRecoilHelper->RecoilStart();
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

void AFPCGun::TryBeginReload()
{
	if (!bIsReloading && RemainingBulletsInMag < GunSettings.MagCapacity)
	{
		bIsReloading = true;
		OnReloadStarted.Broadcast(RemainingBulletsInMag == 0, this);
	}
}

void AFPCGun::OnMagReloadFinishedPlaying()
{
	RemainingMagazines--;
	bIsReloading = false;
	OnReloadFinished.Broadcast(this);
	SetRemainingBulletsInMag(GunSettings.MagCapacity);
}

void AFPCGun::SetRemainingBulletsInMag(int InRemainingBullets)
{
	RemainingBulletsInMag = InRemainingBullets;

	if (OnRemainingBulletsChanged.IsBound())
	{
		OnRemainingBulletsChanged.Broadcast(RemainingBulletsInMag);
	}
}

TArray<TObjectPtr<UMeshComponent>> AFPCGun::GatherWeaponMeshComps()
{
	return TArray<TObjectPtr<UMeshComponent>>{ReceiverMeshComp, MagazineMeshComp, MuzzleMeshComp, OpticMeshComp, IronSightMeshComp};
}

void AFPCGun::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Construct the recoil helper for the weapon
	GunRecoilHelper = NewObject<URecoilHelper>(this, URecoilHelper::StaticClass());
	GunRecoilHelper->RecoilCurve = GunSettings.RecoilSettings.RecoilCurve;
	GunRecoilHelper->FireRate = 1 / GunSettings.FireRate;
	GunRecoilHelper->RecoveryTime = GunSettings.RecoilSettings.RecoveryTime;
	GunRecoilHelper->RecoverySpeed = GunSettings.RecoilSettings.RecoverySpeed;
	GunRecoilHelper->RecoilStrengthMultiplier = GunSettings.RecoilSettings.RecoilStrengthMultiplier;

	// Get required Socket Transforms
	AimSocketActorSpaceTransform = OpticMeshComp->GetSocketTransform(TEXT("SOCKET_Aim"), RTS_Actor);
	EmitterSocketActorSpaceTransform = MuzzleMeshComp->GetSocketTransform(TEXT("SOCKET_Emitter"), RTS_Actor);

	// Initial gun settings
	SetRemainingBulletsInMag(GunSettings.MagCapacity);

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
	{
		if (!bWasTriggerLiftedAfterLastFire)
		{
			// Try starting a reload when the mag is empty
			if (RemainingBulletsInMag == 0)
				TryBeginReload();

			switch (GunSettings.FireMode)
			{
			case Automatic:
				{
					GunRecoilHelper->RecoilStop();
					break;
				}
			default: ;
			}
		}

		bWasTriggerLiftedAfterLastFire = true;
	}

	if (GunRecoilHelper)
		GunRecoilHelper->RecoilTick(DeltaSeconds);
}

void AFPCGun::SetupWeapon(const ECameraMode TargetCameraMode, USceneComponent* AttachCharacterMesh)
{
	Super::SetupWeapon(TargetCameraMode, AttachCharacterMesh);

	// Pool the bullets
	if (BulletClass && OwningOperator->WorldObjectPool)
	{
		FPooledActorSettings BulletPoolSettings;
		BulletPoolSettings.bCanExpand = false;
		BulletPoolSettings.InitialSpawnCount = GunSettings.MagCapacity;
		OwningOperator->WorldObjectPool->AddActorType(BulletClass, BulletPoolSettings);
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
		BulletSpawnTransform = EmitterSocketTransform;

		/*
		 * Uncomment this if the bullet needs to be spawned from the aim socket's height' but at the emitter's distance'
		 */
		// const FTransform AimSocketTransform = OpticMeshComp->GetSocketTransform(TEXT("SOCKET_Aim"));
		// const FVector RelativeEmitterLocation = AimSocketTransform.InverseTransformPosition(EmitterSocketTransform.GetLocation());
		// const FVector FinalSpawnLocation = AimSocketTransform.TransformPosition(FVector(0, RelativeEmitterLocation.Y, 0));
		// BulletSpawnTransform.SetLocation(FinalSpawnLocation);
	}
	else if (UsedInCameraMode == ECameraMode::TPS)
	{
		BulletSpawnTransform = EmitterSocketTransform;
	}

	if (UsedInCameraMode == OwningCharacterCameraManager->GetCurrentCameraMode())
	{
		// This is the direction vector that points directly at the point the user is looking at
		FVector AccurateFireDirectionVector = OwningCharacterCameraManager->GetCurrentCameraLookAtHit() - BulletSpawnTransform.GetLocation();

		// Now we add inaccuracy to it using the gun settings depending on whether it's aiming down sight.
		float BulletSpreadAngle = OwningCharacterWeaponManager->GetIsCharacterInADSState() ? GunSettings.BulletSpreadAngle_Aiming : GunSettings.BulletSpreadAngle;
		float HalfAngleRad = FMath::DegreesToRadians(BulletSpreadAngle * 0.5f);
		FVector SpreadFireDirection = FMath::VRandCone(AccurateFireDirectionVector, HalfAngleRad).GetSafeNormal();

		FRotator BulletRotation = FRotationMatrix::MakeFromY(SpreadFireDirection).Rotator();
		BulletSpawnTransform.SetRotation(BulletRotation.Quaternion());

		if (AFPCBullet* NewBullet = AcquireBullet())
			NewBullet->PropelBullet(BulletSpawnTransform, GunSettings.BulletVelocity);
	}

	// Shot count
	SetRemainingBulletsInMag(RemainingBulletsInMag - 1);
	if (RemainingBulletsInMag == 0)
		OnMagWasEmptied.Broadcast(this);

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
		// Stop Recoil
		GunRecoilHelper->RecoilStop();
		GetWorld()->GetTimerManager().SetTimer(GunCoolDownHandle, [this]()
		{
			bIsWeaponInCoolDown = false;
		}, GunSettings.FireCoolDownInterval, false);
	}
}

AFPCBullet* AFPCGun::AcquireBullet()
{
	AActor* BulletActorFromPool;
	OwningOperator->WorldObjectPool->Pull(BulletClass, BulletActorFromPool);
	if (AFPCBullet* NewBullet = Cast<AFPCBullet>(BulletActorFromPool))
	{
		NewBullet->SetOwners(OwningOperator.Get(), this);
		return NewBullet;
	}

	return nullptr;
}
