// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Actor/FPCCharacter.h"
#include "FPCEnemyCharacter.generated.h"

class AFPCOperator;
class UFPCEnemyAnimInstance;

UCLASS(Abstract)
class FPEACE_API AFPCEnemyCharacter : public AFPCCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCEnemyCharacter();

	void OnTookDamage(TWeakObjectPtr<AFPCOperator> From, FName HitBone);

	void OnDeath();

	// ----------------- GETTERS -------------------
	TWeakObjectPtr<class UFPCCharacterMovementComponent> GetEnemyMovementComponent() const { return EnemyMovementComponent; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE")
	int32 KillPoints = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|HitReactions")
	TObjectPtr<UAnimMontage> HitReaction_Head;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|HitReactions")
	TObjectPtr<UAnimMontage> HitReaction_LeftHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|HitReactions")
	TObjectPtr<UAnimMontage> HitReaction_LeftLeg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|HitReactions")
	TObjectPtr<UAnimMontage> HitReaction_RightHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|HitReactions")
	TObjectPtr<UAnimMontage> HitReaction_RightLeg;

	UPROPERTY()
	TWeakObjectPtr<UFPCEnemyAnimInstance> BaseAnimInstance;

	virtual void PostInitializeComponents() override;

private:
	TWeakObjectPtr<UFPCCharacterMovementComponent> EnemyMovementComponent;
};
