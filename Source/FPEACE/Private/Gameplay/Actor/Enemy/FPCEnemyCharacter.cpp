// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCEnemyCharacter.h"
#include "AbilitySystemGlobals.h"
#include "FCTween.h"
#include "GameplayAbilitiesModule.h"
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
	LeftHandHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandHitBox"));

	EnemyMovementComponent = Cast<UFPCCharacterMovementComponent>(ACharacter::GetMovementComponent());
}

void AFPCEnemyCharacter::OnDeath_Implementation()
{
	Super::OnDeath_Implementation();

	EnemyMovementComponent->DisableMovement();
	FPCAbilitySystemComponent->CancelAllAbilities();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MainBodyMeshComp->SetCollisionProfileName("Ragdoll");
	MainBodyMeshComp->SetSimulatePhysics(true);
	HealthBarWidgetComp->SetHiddenInGame(true);

	FCTween::Play(-0.4f, 0.7f, [&](float V)
	{
		MainMeshDynamicMaterial->SetScalarParameterValue(FName("DissolveAmount"), V);
	}, 5)->SetOnComplete([&]
	{
		// Notify this enemy's death
		OnEnemyDeath.Broadcast(this);
	});
}

void AFPCEnemyCharacter::OnPulledFromPool_Implementation()
{
	IObjectPooledActor::OnPulledFromPool_Implementation();

	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(FPCAbilitySystemComponent, ContentID, 1, true);

	MainMeshDynamicMaterial->SetScalarParameterValue(FName("DissolveAmount"), -0.4f);
	GetCapsuleComponent()->SetCollisionProfileName("FPC_EnemyCapsule");
	MainBodyMeshComp->SetCollisionProfileName("FPC_EnemyBody");
	HealthBarWidgetComp->SetHiddenInGame(false);
	EnemyMovementComponent->SetMovementMode(MOVE_Walking);
	GetCapsuleComponent()->SetEnableGravity(true);
	MainBodyMeshComp->SetEnableGravity(true);
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}

void AFPCEnemyCharacter::OnPushedToPool_Implementation()
{
	IObjectPooledActor::OnPushedToPool_Implementation();

	SetActorTransform(FTransform::Identity);

	FPCAbilitySystemComponent->CancelAllAbilities();
	EnemyMovementComponent->DisableMovement();
	MainBodyMeshComp->SetSimulatePhysics(false);

	GetCapsuleComponent()->SetEnableGravity(false);
	MainBodyMeshComp->SetEnableGravity(false);
	MainBodyMeshComp->SetRelativeTransform(DefaultMainMeshRelativeTransform);
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}

void AFPCEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (MainBodyMeshComp->GetSkeletalMeshAsset())
		RightHandHitBox->AttachToComponent(MainBodyMeshComp, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), FName(TEXT("Hand_R")));

	if (MainBodyMeshComp->GetSkinnedAsset())
		LeftHandHitBox->AttachToComponent(MainBodyMeshComp, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), FName(TEXT("Hand_L")));

	BaseAnimInstance = Cast<UFPCEnemyAnimInstance>(MainBodyMeshComp->GetAnimInstance());
	LeftHandHitBox->OnComponentBeginOverlap.AddDynamic(this, &AFPCEnemyCharacter::HandHit);
	RightHandHitBox->OnComponentBeginOverlap.AddDynamic(this, &AFPCEnemyCharacter::HandHit);
}

void AFPCEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AttackAbility)
		FPCAbilitySystemComponent->K2_GiveAbility(AttackAbility);

	if (!MainMeshDynamicMaterial.IsValid())
		MainMeshDynamicMaterial = MainBodyMeshComp->CreateAndSetMaterialInstanceDynamic(0);

	if (!PlayerOperator.IsValid())
		PlayerOperator = Cast<AFPCOperator>(GetWorld()->GetFirstPlayerController()->GetPawn());

	DefaultMainMeshRelativeTransform = MainBodyMeshComp->GetRelativeTransform();
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
