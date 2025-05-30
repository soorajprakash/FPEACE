// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "MenuOperatorLevelStage.h"
#include "FPCMenuOperator.h"


// Sets default values
AMenuOperatorLevelStage::AMenuOperatorLevelStage()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (!OperatorActor)
		OperatorActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("Operator"));
}

void AMenuOperatorLevelStage::SetOperatorActorClass(const TSubclassOf<AActor>& ActorClass) const
{
	if (OperatorActor)
		OperatorActor->SetChildActorClass(ActorClass);
}
