// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCEnemyHitNotifyState.h"
#include "Gameplay/Actor/Enemy/FPCEnemyCharacter.h"

void UFPCEnemyHitNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!EnemyCharacterRef.IsValid())
		EnemyCharacterRef = Cast<AFPCEnemyCharacter>(MeshComp->GetOwner());

	if (EnemyCharacterRef.IsValid())
		EnemyCharacterRef.Get()->SetCanDamagePlayer(true);
}

void UFPCEnemyHitNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (EnemyCharacterRef.IsValid())
		EnemyCharacterRef->SetCanDamagePlayer(false);
}
