// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCPooledActor.h"


// Sets default values
AFPCPooledActor::AFPCPooledActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsTickable = CanEverTick();
}

void AFPCPooledActor::OnPushedToPool_Implementation()
{
	IObjectPooledActor::OnPushedToPool_Implementation();

	PrimaryActorTick.bCanEverTick = false;
}

void AFPCPooledActor::OnPulledFromPool_Implementation()
{
	IObjectPooledActor::OnPulledFromPool_Implementation();

	if (bIsTickable)
		PrimaryActorTick.bCanEverTick = true;
}
