// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCEnemyCharacter.h"
#include "FCTween.h"
#include "Components/WidgetComponent.h"
#include "Gameplay/Actor/Operator/FPCOperator.h"
#include "Gameplay/AnimInstanceClasses/Enemy/FPCEnemyAnimInstance.h"
#include "Gameplay/ExtendedClasses/FPCPlayerState.h"
#include "Gameplay/ExtendedClasses/Components/FPCAbilitySystemComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCCharacterMovementComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"
#include "Gameplay/GAS/AttribueSets/FPCHealthAttributeSet.h"


// Sets default values
AFPCEnemyCharacter::AFPCEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidgetComp->SetupAttachment(RootComponent);
	
	EnemyMovementComponent = Cast<UFPCCharacterMovementComponent>(ACharacter::GetMovementComponent());
}

void AFPCEnemyCharacter::OnReceivedDamage(TWeakObjectPtr<AFPCOperator> From, FName HitBone)
{
	if (HealthAttributeSet->GetHealth() > 0)
	{
		if (BaseAnimInstance.IsValid())
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

			BaseAnimInstance->OnHitDamageTaken(SelectedMontage);
		}
	}
	else
	{
		// Add score for player
		if (AFPCPlayerState* PS = Cast<AFPCPlayerState>(From->GetPlayerState()))
			PS->AddScore(KillPoints);

		OnDeath();
	}
}

void AFPCEnemyCharacter::OnDeath()
{
	EnemyMovementComponent->DisableMovement();
	FPCAbilitySystemComponent->CancelAllAbilities();
	MainBodyMeshComp->SetCollisionProfileName("Ragdoll");
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
}
