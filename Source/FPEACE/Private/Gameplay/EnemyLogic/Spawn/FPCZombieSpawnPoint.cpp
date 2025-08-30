// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPCZombieSpawnPoint.h"
#include "Components/BoxComponent.h"


// Sets default values
AFPCZombieSpawnPoint::AFPCZombieSpawnPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (!SpawnCollisionDetector)
	{
		SpawnCollisionDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnCollisionDetector"));
		SpawnCollisionDetector->SetupAttachment(RootComponent);
		SpawnCollisionDetector->SetGenerateOverlapEvents(true);
		SpawnCollisionDetector->SetCollisionProfileName("FPC_SpawnPoint");
	}
}

void AFPCZombieSpawnPoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (SpawnCollisionDetector)
	{
		SpawnCollisionDetector->OnComponentBeginOverlap.AddDynamic(this, &AFPCZombieSpawnPoint::SomethingEnteredSpawnPointArea);
		SpawnCollisionDetector->OnComponentEndOverlap.AddDynamic(this, &AFPCZombieSpawnPoint::SomethingExitedSpawnPointArea);
	}
}

void AFPCZombieSpawnPoint::SetSpawnBusy(bool bIsOccupied)
{
	bIsSpawnBusy = bIsOccupied;
	OnSpawnPointOccupancyChanged.Broadcast(this, bIsSpawnBusy);
}

void AFPCZombieSpawnPoint::SomethingEnteredSpawnPointArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                          const FHitResult& SweepResult)
{
	OverlapCounter++;
	SetSpawnBusy(true);
}

void AFPCZombieSpawnPoint::SomethingExitedSpawnPointArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlapCounter = FMath::Max(0, OverlapCounter - 1);
	SetSpawnBusy(OverlapCounter > 0);
}
