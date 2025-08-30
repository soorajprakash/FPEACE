// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.
#pragma once

#include "CoreMinimal.h"
#include "ObjectPoolSubsystem.h"
#include "GameFramework/Actor.h"
#include "Gameplay/ExtendedClasses/FPCActor.h"
#include "FPCZombieStreamSpawner.generated.h"

class AFPCEnemyCharacter;
class AFPCZombieSpawnPoint;

UCLASS()
class AFPCZombieStreamSpawner : public AFPCActor
{
	GENERATED_BODY()

public:
	// ---- Gameplay rules ----
	// Zombies to spawn initially
	UPROPERTY(EditAnywhere, Category="Zombies", meta=(ClampMin="0"))
	int32 DesiredAliveCount = 20;

	// Call back from Zombie class when it dies
	UFUNCTION(Category="Zombies")
	void OnZombieDeath(AFPCEnemyCharacter* Zombie);

protected:
	UPROPERTY(EditAnywhere, Category="Zombies")
	TArray<AFPCZombieSpawnPoint*> SpawnPoints;

	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UObjectPool* ZombiePool;

	UPROPERTY()
	TArray<AFPCZombieSpawnPoint*> AvailableSpawnPoints;

	// Provide multiple Character (or your Zombie base) classes
	UPROPERTY(EditAnywhere, Category="Zombies")
	TArray<TSubclassOf<AFPCEnemyCharacter>> ZombieClasses;

	// ---- Runtime state ----
	// Track currently alive zombies we spawned
	TSet<TWeakObjectPtr<AFPCEnemyCharacter>> ActiveZombies;

	UFUNCTION()
	void SpawnPointStateChanged(TWeakObjectPtr<AFPCZombieSpawnPoint> SourcePoint, bool bIsOccupied);

	bool TryActivateOne();

	// Pick a random spawn point that is not cooling down
	bool PickRandomAvailableSpawnPoint(FVector& OutLoc, FRotator& OutRot, TWeakObjectPtr<AFPCZombieSpawnPoint>& OutPoint);
};
