// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPCActor.generated.h"

UCLASS()
class FPEACE_API AFPCActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPCActor();

	UFUNCTION()
	void ToggleActor(bool bActive);
};
