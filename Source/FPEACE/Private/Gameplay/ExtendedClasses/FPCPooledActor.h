// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "ObjectPooledActorInterface.h"
#include "GameFramework/Actor.h"
#include "FPCPooledActor.generated.h"

class UObjectPool;

UCLASS()
class FPEACE_API AFPCPooledActor : public AActor, public IObjectPooledActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPCPooledActor();

protected:
	
	bool bIsTickable = false;
	
	virtual void OnPushedToPool_Implementation() override;
	virtual void OnPulledFromPool_Implementation() override;
};
