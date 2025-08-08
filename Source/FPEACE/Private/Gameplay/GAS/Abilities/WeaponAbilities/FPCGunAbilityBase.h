// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FPCGunAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class FPEACE_API UFPCGunAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFPCGunAbilityBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<class AFPCGun> GunRef;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
