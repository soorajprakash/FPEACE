// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FPCEnemyAbilityBase.generated.h"

class UFPCSkeletalMeshComponent;
class AFPCEnemyCharacter;
class AFPCEnemyAIController;
/**
 * 
 */
UCLASS(Abstract)
class FPEACE_API UFPCEnemyAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

	UFPCEnemyAbilityBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<AFPCEnemyAIController> FPCAIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<UFPCSkeletalMeshComponent> EnemyMainBodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<AFPCEnemyCharacter> FPCEnemyCharacter;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
