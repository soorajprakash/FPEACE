// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCCharacter.h"
#include "FPCCapsuleComponent.h"
#include "FPCCharacterMovementComponent.h"
#include "FPCGameplayPlayerController.h"
#include "Gameplay/FPCSkeletalMeshComponent.h"
#include "ObjectPoolSubsystem.h"

// Sets default values
AFPCCharacter::AFPCCharacter(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer.SetDefaultSubobjectClass<UFPCSkeletalMeshComponent>(MeshComponentName).SetDefaultSubobjectClass<UFPCCharacterMovementComponent>(CharacterMovementComponentName).
	                  SetDefaultSubobjectClass<UFPCCapsuleComponent>(CapsuleComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Get references to the Extensions 
	TPSBodyMeshComp = Cast<UFPCSkeletalMeshComponent>(GetMesh());
	TPSBodyMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	TPSBodyMeshComp->SetCastHiddenShadow(true);
}

void AFPCCharacter::PossessedBy(AController* NewController)
{
	FPCPlayerControllerInstance = CastChecked<AFPCGameplayPlayerController>(NewController);
	Super::PossessedBy(NewController);
}

void AFPCCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (const UWorld* World = GetWorld())
		WorldObjectPool = World->GetSubsystem<UObjectPool>();
}

//	--------------------- GETTER FUNCTIONS ---------------------

TWeakObjectPtr<UFPCSkeletalMeshComponent> AFPCCharacter::GetTPSBodyMeshComp() const
{
	return TPSBodyMeshComp;
}

TWeakObjectPtr<AFPCGameplayPlayerController> AFPCCharacter::GetFPCPlayerController() const
{
	return FPCPlayerControllerInstance;
}
