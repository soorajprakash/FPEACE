// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Actor/FPCCharacter.h"
#include "FPCEnemyCharacter.generated.h"

UCLASS()
class FPEACE_API AFPCEnemyCharacter : public AFPCCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCEnemyCharacter();

	// ----------------- GETTERS -------------------
	TWeakObjectPtr<class UFPCCharacterMovementComponent> GetEnemyMovementComponent() const {return EnemyMovementComponent;}

private:

	TWeakObjectPtr<class UFPCCharacterMovementComponent> EnemyMovementComponent;
};
