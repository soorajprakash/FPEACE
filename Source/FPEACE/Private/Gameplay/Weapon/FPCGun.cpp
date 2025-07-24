// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCGun.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "FPCBullet.h"
#include "ObjectPoolSubsystem.h"
#include "Gameplay/Actor/Operator/FPCOperator.h"
#include "Gameplay/Actor/Operator/Components/FPCOperatorCameraManagerComponent.h"
#include "Gameplay/Actor/Operator/Components/FPCOperatorWeaponManagerComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCAbilitySystemComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCStaticMeshComponent.h"
#include "Gameplay/GAS/Abilities/WeaponAbilities/FPCGunAbility_Fire.h"
#include "Gameplay/GAS/AttribueSets/FPCGunAttributeSet.h"

AFPCGun::AFPCGun()
{
	GunAttributeSet = CreateDefaultSubobject<UFPCGunAttributeSet>(TEXT("GunValueSet"));

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

	if (GunAttributeSet->GetRemainingBulletsInMag() < 1 || bIsReloading)
		return;

	switch (GunSettings.FireMode)
	{
	case SingleShot:
		{
			if (bWasTriggerLiftedAfterLastFire)
				FPCAbilitySystemComponent->TryActivateAbilityByClass(FireAbility);

			break;
		}

	case BurstFire:
		{
			RemainingShotsInBurst = GunAttributeSet->GetFireRate();
			BurstModeFire();
			break;
		}

	case Automatic:
		{
			FPCAbilitySystemComponent->TryActivateAbilityByClass(FireAbility);
			break;
		}
	default: break;
	}
}

void AFPCGun::TryBeginReload()
{
	if (!bIsReloading && GunAttributeSet->GetRemainingBulletsInMag() < GunAttributeSet->GetMagCapacity())
	{
		bIsReloading = true;
		OnReloadStarted.Broadcast(GunAttributeSet->GetRemainingBulletsInMag() == 0, this);
	}
}

void AFPCGun::OnMagReloadFinishedPlaying()
{
	GunAttributeSet->SetRemainingMags(GunAttributeSet->GetRemainingMags() - 1);
	bIsReloading = false;
	OnReloadFinished.Broadcast(this);
	GunAttributeSet->SetRemainingBulletsInMag(GunAttributeSet->GetMagCapacity());
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

	// Initial gun settings
	GunAttributeSet->SetRemainingBulletsInMag(GunAttributeSet->GetMagCapacity());

	// Set up the gun to have no collision
	// TODO : Collision is probably required. Change this to suit the game's needs
	SetActorEnableCollision(false);
}

void AFPCGun::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FPCAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UFPCGunAttributeSet::GetRemainingBulletsInMagAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		if (OnRemainingBulletsChanged.IsBound())
			OnRemainingBulletsChanged.Broadcast(Data.NewValue);
	});

	// Give abilities to the gun
	FPCAbilitySystemComponent->K2_GiveAbility(FireAbility.Get());
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
			if (GunAttributeSet->GetRemainingBulletsInMag() == 0)
				TryBeginReload();
		}

		bWasTriggerLiftedAfterLastFire = true;
	}
}

void AFPCGun::SetupWeapon(const ECameraMode TargetCameraMode, USceneComponent* AttachCharacterMesh)
{
	Super::SetupWeapon(TargetCameraMode, AttachCharacterMesh);

	// Pool the bullets
	if (BulletClass && OwningOperator->WorldObjectPool)
	{
		FPooledActorSettings BulletPoolSettings;
		BulletPoolSettings.bCanExpand = false;
		BulletPoolSettings.InitialSpawnCount = GunAttributeSet->GetMagCapacity();
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

	if (bCameraModeMatchesWeapon)
	{
		// This is the direction vector that points directly at the point the user is looking at
		FVector AccurateFireDirectionVector = OwningCharacterCameraManager->GetCurrentCameraLookAtHit() - BulletSpawnTransform.GetLocation();

		// Now we add inaccuracy to it using the gun settings depending on whether it's aiming down sight.
		float BulletSpreadAngle = OwningCharacterWeaponManager->GetIsCharacterInADSState() ? GunAttributeSet->GetBulletSpreadAngle_ADS() : GunAttributeSet->GetBulletSpreadAngle();
		float HalfAngleRad = FMath::DegreesToRadians(BulletSpreadAngle * 0.5f);
		FVector SpreadFireDirection = FMath::VRandCone(AccurateFireDirectionVector, HalfAngleRad).GetSafeNormal();

		FRotator BulletRotation = FRotationMatrix::MakeFromY(SpreadFireDirection).Rotator();
		BulletSpawnTransform.SetRotation(BulletRotation.Quaternion());

		if (AFPCBullet* NewBullet = AcquireBullet())
			NewBullet->PropelBullet(BulletSpawnTransform, GunAttributeSet->GetBulletVelocity());
	}

	// Shot count
	// SetRemainingBulletsInMag(RemainingBulletsInMag - 1);
	if (GunAttributeSet->GetRemainingBulletsInMag() == 0)
		OnMagWasEmptied.Broadcast(this);

	OnWeaponSuccessfullyUsed.Broadcast(this);
}

void AFPCGun::BurstModeFire()
{
	if (RemainingShotsInBurst > 0)
	{
		FPCAbilitySystemComponent->TryActivateAbilityByClass(FireAbility);
		RemainingShotsInBurst--;

		if (RemainingShotsInBurst > 0)
			GetWorld()->GetTimerManager().SetTimer(GunContinuosFireHandle, this, &AFPCGun::BurstModeFire, GunSettings.BurstFireInterval, false);
		else
			BurstModeFire();
	}
}

AFPCBullet* AFPCGun::AcquireBullet()
{
	AActor* BulletActorFromPool;
	OwningOperator->WorldObjectPool->Pull(BulletClass, BulletActorFromPool);
	if (AFPCBullet* NewBullet = Cast<AFPCBullet>(BulletActorFromPool))
	{
		NewBullet->Initialize(OwningOperator.Get(), this);
		return NewBullet;
	}

	return nullptr;
}
