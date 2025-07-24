// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FPCEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPEACE_API UFPCEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	friend class AFPCEnemyCharacter;

protected:
	/*
	 * Reference to the enemy character that is actively using this anim instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AFPCEnemyCharacter> OwningEnemyCharacter;

	/*
	 * Reference to the movement component in the character that is actively using this anim instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<class UFPCCharacterMovementComponent> OwningCharacterMovementComponent;

	UFUNCTION(BlueprintNativeEvent)
	void OnHitDamageTaken(UAnimMontage* HitReactionMontage);

	virtual void NativeInitializeAnimation() override;
};
