// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCGunAbilityBase.h"
#include "Gameplay/Weapon/FPCGun.h"

UFPCGunAbilityBase::UFPCGunAbilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFPCGunAbilityBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (!GunRef.IsValid())
		GunRef = Cast<AFPCGun>(ActorInfo->AvatarActor);
}
