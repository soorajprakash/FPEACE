// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/ExtendedClasses/FPCActor.h"
#include "MenuOperatorLevelStage.generated.h"


UCLASS()
class FPEACE_API AMenuOperatorLevelStage : public AFPCActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMenuOperatorLevelStage();

	void SetOperatorActorClass(const TSubclassOf<AActor>& ActorClass) const;
protected:
	UPROPERTY(VisibleAnywhere)
	UChildActorComponent* OperatorActor;
};
