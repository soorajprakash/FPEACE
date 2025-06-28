// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/ExtendedClasses/FPCActor.h"
#include "FPCMenuOperator.generated.h"

class UFPCSkeletalMeshComponent;

UCLASS()
class FPEACE_API AFPCMenuOperator : public AFPCActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPCMenuOperator();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UFPCSkeletalMeshComponent> OperatorMesh;
};
