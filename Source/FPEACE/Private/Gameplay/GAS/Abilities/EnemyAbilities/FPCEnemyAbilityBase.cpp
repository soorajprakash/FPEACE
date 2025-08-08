// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCEnemyAbilityBase.h"
#include "Gameplay/Actor/Enemy/FPCEnemyAIController.h"
#include "Gameplay/Actor/Enemy/FPCEnemyCharacter.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"

UFPCEnemyAbilityBase::UFPCEnemyAbilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFPCEnemyAbilityBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	FPCEnemyCharacter = Cast<AFPCEnemyCharacter>(GetOwningActorFromActorInfo());
	EnemyMainBodyMesh = Cast<UFPCSkeletalMeshComponent>(FPCEnemyCharacter->GetTPSBodyMeshComp());
	FPCAIController = Cast<AFPCEnemyAIController>(FPCEnemyCharacter->GetController());
}
