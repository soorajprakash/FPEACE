// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCActor.h"

// Sets default values
AFPCActor::AFPCActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AFPCActor::ToggleActor(bool bActive)
{
	// 1) Stop or start ticking
	SetActorTickEnabled(bActive);

	// 2) Show or hide in-game (hides all primitive components)
	SetActorHiddenInGame(!bActive);

	// 4) Enable/disable each component (so things like audio, custom components, etc. really turn off)
	TArray<UActorComponent*> Comps;
	GetComponents(Comps);
	for (UActorComponent* Comp : Comps)
	{
		// De/activate UActorComponents
		if (bActive)
			Comp->Activate(/*bReset=*/false);
		else
			Comp->Deactivate();

		// If it’s a scene component, also turn off visibility
		if (USceneComponent* SceneComp = Cast<USceneComponent>(Comp))
			SceneComp->SetVisibility(bActive, true);
	}
}
