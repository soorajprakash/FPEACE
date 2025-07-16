// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCCharacter.h"
#include "FPCGameplayPlayerController.h"
#include "ObjectPoolSubsystem.h"
#include "DataStructures/FPCCharacterData.h"
#include "Gameplay/ExtendedClasses/FPCGameInstance.h"
#include "Gameplay/ExtendedClasses/Components/FPCAbilitySystemComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCCapsuleComponent.h"
#include "Operator/Components/FPCOperatorMovementComponent.h"
#include "Gameplay/GAS/Abilities/FPCCharacterAbilityBase.h"

// Sets default values
AFPCCharacter::AFPCCharacter(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer.SetDefaultSubobjectClass<UFPCSkeletalMeshComponent>(MeshComponentName).SetDefaultSubobjectClass<UFPCOperatorMovementComponent>(CharacterMovementComponentName).
	                  SetDefaultSubobjectClass<UFPCCapsuleComponent>(CapsuleComponentName))
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Get references to the Extensions 
	MainBodyMeshComp = Cast<UFPCSkeletalMeshComponent>(GetMesh());
	MainBodyMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	MainBodyMeshComp->SetCastHiddenShadow(true);

	// Construct components
	if (!FPCAbilitySystemComponent)
		FPCAbilitySystemComponent = CreateDefaultSubobject<UFPCAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void AFPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (FPCAbilitySystemComponent)
	{
		FPCAbilitySystemComponent->InitAbilityActorInfo(this, this);

		// Grant basic abilities
		for (TSubclassOf Ability : BasicAbilities)
		{
			FPCAbilitySystemComponent->K2_GiveAbility(Ability);
			FPCAbilitySystemComponent->TryActivateAbilityByClass(Ability);
		}
	}
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

void AFPCCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	GetCharacterData();
}

//	--------------------- GETTER FUNCTIONS ---------------------

TWeakObjectPtr<UFPCOperatorData> AFPCCharacter::GetCharacterData()
{
	// Get the Character Data asset reference
	if (FPCCharacterData == nullptr)
		if (UFPCGameInstance* FPCGameInstance = UFPCGameInstance::GetInstance(this))
			FPCCharacterData = FPCGameInstance->CharacterData.LoadSynchronous();

	return FPCCharacterData;
}

TWeakObjectPtr<UFPCSkeletalMeshComponent> AFPCCharacter::GetTPSBodyMeshComp() const
{
	return MainBodyMeshComp;
}

TWeakObjectPtr<AFPCGameplayPlayerController> AFPCCharacter::GetFPCPlayerController() const
{
	return FPCPlayerControllerInstance;
}

UAbilitySystemComponent* AFPCCharacter::GetAbilitySystemComponent() const
{
	return FPCAbilitySystemComponent;
}
