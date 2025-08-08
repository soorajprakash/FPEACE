// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/ExtendedClasses/FPCActor.h"
#include "FPCZombieSpawner.generated.h"

UCLASS()
class FPEACE_API AFPCZombieSpawner : public AFPCActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPCZombieSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FPEACE")
	TArray<TObjectPtr<class AFPCZombieSpawnPoint>> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FPEACE")
	TSubclassOf<class AFPCEnemyCharacter> ZombieClass;

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<AFPCZombieSpawnPoint> GetRandomSpawnPoint();
};
