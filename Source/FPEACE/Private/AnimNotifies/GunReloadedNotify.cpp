// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "GunReloadedNotify.h"
#include "Gameplay/Weapon/FPCGun.h"

void UGunReloadedNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (AFPCGun* TargetGun = Cast<AFPCGun>(MeshComp->GetOwner()))
			TargetGun->OnMagReloadFinishedPlaying();
	}
}
