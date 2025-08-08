// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCCharacter.h"
#include "AbilitySystemGlobals.h"
#include "GameplayAbilitiesModule.h"
#include "MetasoundSource.h"
#include "ObjectPoolSubsystem.h"
#include "Components/AudioComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCAbilitySystemComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCCapsuleComponent.h"
#include "Operator/Components/FPCOperatorMovementComponent.h"
#include "Gameplay/GAS/Abilities/OperatorAbilities/FPCOperatorAbilityBase.h"
#include "Gameplay/GAS/AttribueSets/FPCHealthAttributeSet.h"

// Sets default values
AFPCCharacter::AFPCCharacter(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer.SetDefaultSubobjectClass<UFPCSkeletalMeshComponent>(MeshComponentName).SetDefaultSubobjectClass<UFPCCharacterMovementComponent>(CharacterMovementComponentName).
	                  SetDefaultSubobjectClass<UFPCCapsuleComponent>(CapsuleComponentName))
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Get references to the Extensions 
	MainBodyMeshComp = Cast<UFPCSkeletalMeshComponent>(GetMesh());
	MainBodyMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	MainBodyMeshComp->SetCastHiddenShadow(true);

	// Construct components
	VocalAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Vocal Audio Component"));
	VocalAudioComponent->SetupAttachment(MainBodyMeshComp, FName("Head"));
	VocalAudioComponent->SetAutoActivate(true);

	if (!FPCAbilitySystemComponent)
		FPCAbilitySystemComponent = CreateDefaultSubobject<UFPCAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	HealthAttributeSet = CreateDefaultSubobject<UFPCHealthAttributeSet>(TEXT("HealthSet"));
}

void AFPCCharacter::OnHitDamageTaken_Implementation(UAnimMontage* HitReactionMontage)
{
	
}

void AFPCCharacter::PlayVocalSound(USoundBase* Sound, float CrossfadeDuration)
{
	if (!VocalAudioComponent)
		return;

	if (UMetaSoundSource* MS = Cast<UMetaSoundSource>(Sound))
	{
		VocalAudioComponent->SetSound(MS);
		VocalAudioComponent->Play();
	}
	else
	{
		if (VocalAudioComponent->GetSound() != VocalCrossfader)
			VocalAudioComponent->SetSound(VocalCrossfader);

		VocalAudioComponent->SetObjectParameter(FName("NewAudio"), Sound);

		if (VocalAudioComponent->IsPlaying())
			VocalAudioComponent->SetTriggerParameter(FName("TriggerCrossfade"));
		else
			VocalAudioComponent->Play();
	}
}

void AFPCCharacter::OnDeath_Implementation()
{
}

void AFPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (FPCAbilitySystemComponent)
	{
		// Grant basic abilities
		for (TSubclassOf Ability : BasicAbilities)
		{
			FPCAbilitySystemComponent->K2_GiveAbility(Ability);
			FPCAbilitySystemComponent->TryActivateAbilityByClass(Ability);
		}
	}
}

void AFPCCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FPCAbilitySystemComponent->InitAbilityActorInfo(this, this);

	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(FPCAbilitySystemComponent, ContentID, 1, true);
}

void AFPCCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (const UWorld* World = GetWorld())
		WorldObjectPool = World->GetSubsystem<UObjectPool>();
}

void AFPCCharacter::OnReceivedDamage(TWeakObjectPtr<AFPCCharacter> From, FName HitBone)
{
}

//	--------------------- GETTER FUNCTIONS ---------------------

TWeakObjectPtr<UFPCSkeletalMeshComponent> AFPCCharacter::GetTPSBodyMeshComp() const
{
	return MainBodyMeshComp;
}

UAbilitySystemComponent* AFPCCharacter::GetAbilitySystemComponent() const
{
	return FPCAbilitySystemComponent;
}
