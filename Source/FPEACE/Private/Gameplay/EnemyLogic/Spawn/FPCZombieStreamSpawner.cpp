// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPCZombieStreamSpawner.h"
#include "FPCZombieSpawnPoint.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "ObjectPoolSubsystem.h"
#include "Gameplay/Actor/Enemy/FPCEnemyCharacter.h"

bool AFPCZombieStreamSpawner::TryActivateOne()
{
	if (ZombieClasses.Num() == 0 || SpawnPoints.Num() == 0 || !ZombiePool) return false;

	// choose a class uniformly
	const int32 ClassIdx = FMath::RandRange(0, ZombieClasses.Num() - 1);
	TSubclassOf<AFPCEnemyCharacter> ChosenClass = ZombieClasses[ClassIdx];
	if (!*ChosenClass) return false;

	// Pull an actor from the pool
	AActor* SpawnedActor = nullptr;
	if (!ZombiePool->Pull(ChosenClass, /*out*/ SpawnedActor) || !SpawnedActor)
	{
		return false; // pool empty / cannot expand
	}

	// Bind death → push back + request replacement (keep it simple)
	if (AFPCEnemyCharacter* Enemy = Cast<AFPCEnemyCharacter>(SpawnedActor))
	{
		Enemy->OnEnemyDeath.RemoveAll(this);
		Enemy->OnEnemyDeath.AddDynamic(this, &AFPCZombieStreamSpawner::OnZombieDeath);
	}

	// Pick a random spawn point that is NOT cooling down
	FVector SpawnLoc;
	FRotator SpawnRot;
	TWeakObjectPtr<AFPCZombieSpawnPoint> ChosenPoint;
	if (!PickRandomAvailableSpawnPoint(SpawnLoc, SpawnRot, ChosenPoint))
	{
		ZombiePool->Push(SpawnedActor); // no free point this frame, try next tick
		return false;
	}

	// Place EXACTLY at spawn point location/rotation (no nav, no sweep)
	ChosenPoint->SetSpawnBusy(true);
	SpawnedActor->SetActorLocationAndRotation(SpawnLoc, SpawnRot, /*bSweep*/false, nullptr, ETeleportType::TeleportPhysics);

	// Track alive
	if (AFPCEnemyCharacter* Z = Cast<AFPCEnemyCharacter>(SpawnedActor))
		ActiveZombies.Add(Z);

	return true;
}

void AFPCZombieStreamSpawner::SpawnPointStateChanged(TWeakObjectPtr<AFPCZombieSpawnPoint> SourcePoint, bool bIsOccupied)
{
	if (!SourcePoint.IsValid())
		return;

	if (bIsOccupied)
		AvailableSpawnPoints.Remove(SourcePoint.Get());
	else
		AvailableSpawnPoints.AddUnique(SourcePoint.Get());
}

bool AFPCZombieStreamSpawner::PickRandomAvailableSpawnPoint(FVector& OutLoc, FRotator& OutRot, TWeakObjectPtr<AFPCZombieSpawnPoint>& OutPoint)
{
	if (AvailableSpawnPoints.Num() == 0)
		return false;

	const int32 ClassIdx = FMath::RandRange(0, AvailableSpawnPoints.Num() - 1);
	if (AFPCZombieSpawnPoint* Chosen = AvailableSpawnPoints[ClassIdx])
	{
		OutLoc = Chosen->GetActorLocation();
		OutRot = Chosen->GetActorRotation();
		OutPoint = Chosen;
		return true;
	}

	return false;
}

void AFPCZombieStreamSpawner::OnZombieDeath(AFPCEnemyCharacter* Zombie)
{
	if (!Zombie || !ZombiePool) return;

	if (AFPCEnemyCharacter* Z = Cast<AFPCEnemyCharacter>(Zombie))
		ActiveZombies.Remove(Z);

	ZombiePool->Push(Zombie); // back to pool; do NOT Destroy()

	// Replace the zombie
	TryActivateOne();
}

void AFPCZombieStreamSpawner::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	AvailableSpawnPoints = SpawnPoints;
}

void AFPCZombieStreamSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		ZombiePool = World->GetSubsystem<UObjectPool>();

		for (TSubclassOf ZombieClass : ZombieClasses)
			ZombiePool->AddActorType(ZombieClass, FPooledActorSettings());
	}

	for (AFPCZombieSpawnPoint* SpawnPoint : SpawnPoints)
		SpawnPoint->OnSpawnPointOccupancyChanged.AddDynamic(this, &AFPCZombieStreamSpawner::SpawnPointStateChanged);

	//Initial Spawn
	for (int i = 0; i < DesiredAliveCount; i++)
		TryActivateOne();
}
