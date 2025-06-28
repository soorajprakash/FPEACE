// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCMuzzleFlashNotify.h"
#include "Gameplay/Weapon/FPCGun.h"

void UFPCMuzzleFlashNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
		if (AFPCGun* OwningGun = Cast<AFPCGun>(MeshComp->GetOwner()))
			OwningGun->InitiateMuzzleFlash();
}
