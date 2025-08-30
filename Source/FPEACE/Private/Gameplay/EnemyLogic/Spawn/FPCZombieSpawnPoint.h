// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "FPCZombieSpawnPoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpawnPointStateChanged, TWeakObjectPtr<AFPCZombieSpawnPoint>, SourcePoint, bool, bIsOccupied);

class UBoxComponent;

UCLASS()
class FPEACE_API AFPCZombieSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	/*
	 * Whether this spawn point is currently being used
	 */
	UPROPERTY(VisibleAnywhere, Category="FPEACE")
	bool bIsSpawnBusy = false;

	FOnSpawnPointStateChanged OnSpawnPointOccupancyChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category="FPEACE")
	TObjectPtr<UBoxComponent> SpawnCollisionDetector;

	// Sets default values for this actor's properties
	AFPCZombieSpawnPoint();
	
	virtual void PostInitializeComponents() override;

private:
	int32 OverlapCounter = 0;

	void SetSpawnBusy(bool bIsOccupied);

	UFUNCTION()
	void SomethingEnteredSpawnPointArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                    const FHitResult& SweepResult);

	UFUNCTION()
	void SomethingExitedSpawnPointArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
