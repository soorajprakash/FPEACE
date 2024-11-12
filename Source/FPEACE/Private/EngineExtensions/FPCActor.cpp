// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCActor.h"

// Sets default values
AFPCActor::AFPCActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AFPCActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPCActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

