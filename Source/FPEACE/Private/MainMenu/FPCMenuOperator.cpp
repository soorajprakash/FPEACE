// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCMenuOperator.h"

#include "Gameplay/FPCSkeletalMeshComponent.h"


// Sets default values
AFPCMenuOperator::AFPCMenuOperator()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (!OperatorMesh)
		OperatorMesh = CreateDefaultSubobject<UFPCSkeletalMeshComponent>(TEXT("OperatorMesh"));

	SetRootComponent(OperatorMesh);
}