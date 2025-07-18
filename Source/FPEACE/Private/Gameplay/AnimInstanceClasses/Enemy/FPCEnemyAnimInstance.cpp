// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCEnemyAnimInstance.h"
#include "Gameplay/Actor/Enemy/FPCEnemyCharacter.h"

void UFPCEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningEnemyCharacter == nullptr)
		OwningEnemyCharacter = Cast<AFPCEnemyCharacter>(TryGetPawnOwner());

	if (OwningEnemyCharacter.IsValid())
	{
		if (!OwningCharacterMovementComponent.IsValid())
			OwningCharacterMovementComponent = OwningEnemyCharacter->GetEnemyMovementComponent();
	}
}
