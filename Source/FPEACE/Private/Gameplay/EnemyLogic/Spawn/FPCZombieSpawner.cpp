// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCZombieSpawner.h"
#include "FPCZombieSpawnPoint.h"
#include "Gameplay/Actor/Enemy/FPCEnemyCharacter.h"


// Sets default values
AFPCZombieSpawner::AFPCZombieSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AFPCZombieSpawner::BeginPlay()
{
	Super::BeginPlay();
	const TWeakObjectPtr<AFPCZombieSpawnPoint> SelectedSpawnPoint = GetRandomSpawnPoint();
	if (SelectedSpawnPoint.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("Spawn SpawnPoint"));
		GetWorld()->SpawnActor<AFPCEnemyCharacter>(ZombieClass, SelectedSpawnPoint->GetActorTransform());
	}
}

TWeakObjectPtr<AFPCZombieSpawnPoint> AFPCZombieSpawner::GetRandomSpawnPoint()
{
	return SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];
}
