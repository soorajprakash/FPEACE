// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "FPCEnemyAIController.generated.h"

UCLASS()
class FPEACE_API AFPCEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPCEnemyAIController();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPEACE|AI")
	TObjectPtr<class UStateTreeAIComponent> StateTreeAIComp;
};
