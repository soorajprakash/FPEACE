// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "FCTweenInstanceFloat.h"
#include "ObjectPooledActorInterface.h"
#include "Gameplay/Actor/FPCCharacter.h"
#include "FPCEnemyCharacter.generated.h"

class UFPCEnemyCombatAttributeSet;
class UGameplayEffect;
class UBoxComponent;
class UFPCEnemyAbilityBase;
class UWidgetComponent;
class AFPCOperator;
class UFPCEnemyAnimInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyEvent, AFPCEnemyCharacter*, Enemy);

UCLASS(Abstract)
class FPEACE_API AFPCEnemyCharacter : public AFPCCharacter, public IObjectPooledActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCEnemyCharacter();

	FOnEnemyEvent OnEnemyDeath;

	virtual void OnDeath_Implementation() override;

	virtual void OnReceivedDamage(TWeakObjectPtr<AFPCCharacter> From, FName HitBone) override;

	// ----------------- GETTERS -------------------
	TWeakObjectPtr<class UFPCCharacterMovementComponent> GetEnemyMovementComponent() const { return EnemyMovementComponent; }

	// ----------------- SETTERS -------------------
	void SetCanDamagePlayer(bool Value) { bCanDamagePlayer = Value; };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE")
	int32 KillPoints = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE")
	bool bCanDamagePlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE")
	TSubclassOf<UFPCEnemyAbilityBase> AttackAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE")
	TSubclassOf<UGameplayEffect> MeleeDamageEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE")
	TObjectPtr<UFPCEnemyCombatAttributeSet> EnemyCombatAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|HitReactions")
	TObjectPtr<UAnimMontage> HitReaction_Head;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|HitReactions")
	TObjectPtr<UAnimMontage> HitReaction_LeftHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|HitReactions")
	TObjectPtr<UAnimMontage> HitReaction_LeftLeg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|HitReactions")
	TObjectPtr<UAnimMontage> HitReaction_RightHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|HitReactions")
	TObjectPtr<UAnimMontage> HitReaction_RightLeg;

	// ----------------- COMPONENTS -------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBarWidgetComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> RightHandHitBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> LeftHandHitBox;

	UPROPERTY()
	TWeakObjectPtr<UFPCEnemyAnimInstance> BaseAnimInstance;

	// ----------------- INTERFACE IMPLEMENTATIONS -------------------
	virtual void OnPulledFromPool_Implementation() override;
	virtual void OnPushedToPool_Implementation() override;

	// ----------------- OVERRIDES -------------------
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

private:

	/*
	 * The tween used to animate material parameter for dissolve effect
	 */
	FCTweenInstance* DeathMaterialTween;
	
	/*
	 * The initial relative transform of the Main skeletal mesh component
	 */
	FTransform DefaultMainMeshRelativeTransform;
	
	static TWeakObjectPtr<AFPCOperator> PlayerOperator;

	UPROPERTY()
	TWeakObjectPtr<UMaterialInstanceDynamic> MainMeshDynamicMaterial;

	UPROPERTY()
	TWeakObjectPtr<UFPCCharacterMovementComponent> EnemyMovementComponent;

	UFUNCTION()
	void HandHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
