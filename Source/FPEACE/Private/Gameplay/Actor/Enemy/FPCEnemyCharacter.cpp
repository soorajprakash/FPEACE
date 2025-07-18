// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCEnemyCharacter.h"

#include "Gameplay/ExtendedClasses/Components/FPCCharacterMovementComponent.h"


// Sets default values
AFPCEnemyCharacter::AFPCEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	EnemyMovementComponent = Cast<UFPCCharacterMovementComponent>(ACharacter::GetMovementComponent());
}
