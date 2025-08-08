// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCEnemyCharacter.h"
#include "FCTween.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Gameplay/Actor/Operator/FPCOperator.h"
#include "Gameplay/AnimInstanceClasses/Enemy/FPCEnemyAnimInstance.h"
#include "Gameplay/ExtendedClasses/FPCPlayerState.h"
#include "Gameplay/ExtendedClasses/Components/FPCAbilitySystemComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCCharacterMovementComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"
#include "Gameplay/GAS/AttribueSets/FPCHealthAttributeSet.h"
#include "Gameplay/GAS/Abilities/EnemyAbilities/FPCEnemyAbilityBase.h"
#include "Gameplay/GAS/AttribueSets/FPCEnemyCombatAttributeSet.h"
#include "Kismet/KismetSystemLibrary.h"

TWeakObjectPtr<AFPCOperator> AFPCEnemyCharacter::PlayerOperator = nullptr;

// Sets default values
AFPCEnemyCharacter::AFPCEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Setup Attributes
	EnemyCombatAttributeSet = CreateDefaultSubobject<UFPCEnemyCombatAttributeSet>(TEXT("CombatSet"));

	// Setup components
	HealthBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidgetComp->SetupAttachment(RootComponent);

	RightHandHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandHitBox"));
	RightHandHitBox->SetupAttachment(MainBodyMeshComp, FName(TEXT("Hand_R")));

	LeftHandHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandHitBox"));
	LeftHandHitBox->SetupAttachment(MainBodyMeshComp, FName(TEXT("Hand_L")));

	EnemyMovementComponent = Cast<UFPCCharacterMovementComponent>(ACharacter::GetMovementComponent());
}

void AFPCEnemyCharacter::OnDeath_Implementation()
{
	Super::OnDeath_Implementation();

	EnemyMovementComponent->DisableMovement();
	FPCAbilitySystemComponent->CancelAllAbilities();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MainBodyMeshComp->SetCollisionProfileName("Ragdoll");
	MainBodyMeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MainBodyMeshComp->SetSimulatePhysics(true);
	HealthBarWidgetComp->SetHiddenInGame(true);

	UMaterialInstanceDynamic* EnemyMainMat = MainBodyMeshComp->CreateAndSetMaterialInstanceDynamic(0);

	FCTween::Play(-0.4f, 0.7f, [EnemyMainMat](float V)
	{
		EnemyMainMat->SetScalarParameterValue(FName("DissolveAmount"), V);
	}, 5)->SetOnComplete([&]
	{
		// Destroy this actor
	});
}

void AFPCEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BaseAnimInstance = Cast<UFPCEnemyAnimInstance>(MainBodyMeshComp->GetAnimInstance());
	LeftHandHitBox->OnComponentBeginOverlap.AddDynamic(this, &AFPCEnemyCharacter::HandHit);
	RightHandHitBox->OnComponentBeginOverlap.AddDynamic(this, &AFPCEnemyCharacter::HandHit);
}

void AFPCEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AttackAbility)
		FPCAbilitySystemComponent->K2_GiveAbility(AttackAbility);

	if (!PlayerOperator.IsValid())
		PlayerOperator = Cast<AFPCOperator>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void AFPCEnemyCharacter::HandHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bCanDamagePlayer)
	{
		if (AFPCOperator* PlayerCharacter = Cast<AFPCOperator>(OtherActor))
		{
			SetCanDamagePlayer(false);
			FGameplayEffectContextHandle EffectContext = FPCAbilitySystemComponent->MakeEffectContext();
			EffectContext.AddInstigator(this, this);

			FGameplayEffectSpecHandle SpecHandle = FPCAbilitySystemComponent->MakeOutgoingSpec(MeleeDamageEffect, 1, EffectContext);

			// Apply Damage Effect to player's ASC
			FPCAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, PlayerCharacter->GetAbilitySystemComponent());

			PlayerCharacter->OnReceivedDamage(this, FName("None"));
		}
	}
}

void AFPCEnemyCharacter::OnReceivedDamage(TWeakObjectPtr<AFPCCharacter> From, FName HitBone)
{
	Super::OnReceivedDamage(From, HitBone);

	if (HealthAttributeSet->GetHealth() > 0)
	{
		// Decide which montage gets played
		UAnimMontage* SelectedMontage;
		FString BoneNameString = HitBone.ToString();

		if (BoneNameString.Contains("arm_l", ESearchCase::IgnoreCase, ESearchDir::FromEnd) || BoneNameString.Contains("hand_l", ESearchCase::IgnoreCase, ESearchDir::FromEnd))
			SelectedMontage = HitReaction_LeftHand;
		else if (BoneNameString.Contains("arm_r", ESearchCase::IgnoreCase, ESearchDir::FromEnd) || BoneNameString.Contains("hand_r", ESearchCase::IgnoreCase, ESearchDir::FromEnd))
			SelectedMontage = HitReaction_RightHand;
		else if (BoneNameString.Contains("thigh_r", ESearchCase::IgnoreCase, ESearchDir::FromEnd) || BoneNameString.Contains("calf_r", ESearchCase::IgnoreCase, ESearchDir::FromEnd) ||
			BoneNameString.
			Contains("foot_r", ESearchCase::IgnoreCase, ESearchDir::FromEnd))
			SelectedMontage = HitReaction_RightLeg;
		else if (BoneNameString.Contains("thigh_l", ESearchCase::IgnoreCase, ESearchDir::FromEnd) || BoneNameString.Contains("calf_l", ESearchCase::IgnoreCase, ESearchDir::FromEnd) ||
			BoneNameString.
			Contains("foot_l", ESearchCase::IgnoreCase, ESearchDir::FromEnd))
			SelectedMontage = HitReaction_LeftLeg;
		else
			SelectedMontage = HitReaction_Head;

		OnHitDamageTaken(SelectedMontage);

		if (PlayerOperator.IsValid())
			PlayerOperator->PlayEnemyHitRegisterSound();
	}
	else
	{
		// Add score for player
		if (AFPCPlayerState* PS = Cast<AFPCPlayerState>(From->GetPlayerState()))
			PS->AddScore(KillPoints);

		if (PlayerOperator.IsValid())
			PlayerOperator->PlayEnemyHitRegisterSound(true);
		OnDeath();
	}
}
