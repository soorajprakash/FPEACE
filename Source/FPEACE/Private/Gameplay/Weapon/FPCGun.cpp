// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCGun.h"
#include "Gameplay/FPCSkeletalMeshComponent.h"
#include "Gameplay/FPCStaticMeshComponent.h"

AFPCGun::AFPCGun()
{
	if (!ReceiverMeshComp)
	{
		ReceiverMeshComp = CreateDefaultSubobject<UFPCSkeletalMeshComponent>(TEXT("ReceiverMeshComp"));
		RootComponent = ReceiverMeshComp;
	}

	if (!MagazineMeshComp)
	{
		MagazineMeshComp = CreateDefaultSubobject<UFPCStaticMeshComponent>(TEXT("MagazineMeshComp"));
		MagazineMeshComp->SetupAttachment(ReceiverMeshComp, FName(TEXT("SOCKET_Magazine")));
	}

	if (!MuzzleMeshComp)
	{
		MuzzleMeshComp = CreateDefaultSubobject<UFPCStaticMeshComponent>(TEXT("MuzzleMeshComp"));
		MuzzleMeshComp->SetupAttachment(ReceiverMeshComp, FName(TEXT("SOCKET_Muzzle")));
	}

	if (!OpticMeshComp)
	{
		OpticMeshComp = CreateDefaultSubobject<UFPCStaticMeshComponent>(TEXT("OpticMeshComp"));
		OpticMeshComp->SetupAttachment(ReceiverMeshComp, FName(TEXT("SOCKET_Scope")));
	}

	if (!IronSightMeshComp)
	{
		IronSightMeshComp = CreateDefaultSubobject<UFPCStaticMeshComponent>(TEXT("IronsightMeshComp"));
		IronSightMeshComp->SetupAttachment(ReceiverMeshComp, FName(TEXT("SOCKET_Default")));
	}
}

TArray<TObjectPtr<UMeshComponent>> AFPCGun::GatherWeaponMeshComps()
{
	return TArray<TObjectPtr<UMeshComponent>>{ReceiverMeshComp, MagazineMeshComp, MuzzleMeshComp, OpticMeshComp, IronSightMeshComp};
}

void AFPCGun::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Set up the gun to have no collision
	// TODO : Collision is probably required. Change this to suit the game's needs
	SetActorEnableCollision(false);
}
