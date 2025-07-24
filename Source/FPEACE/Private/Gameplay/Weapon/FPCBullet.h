// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/ExtendedClasses/FPCPooledActor.h"
#include "FPCBullet.generated.h"

class AFPCOperator;
class UFPCAbilitySystemComponent;
class UAbilitySystemComponent;
class UGameplayEffect;
class AFPCGun;
class UBoxComponent;
class UFPCProjectileMovementComponent;
class UFPCStaticMeshComponent;

UCLASS(BlueprintType)
class FPEACE_API AFPCBullet : public AFPCPooledActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPCBullet();

	void Initialize(AFPCOperator* OwnerCharacter, AFPCGun* OwnerGun);

	void PropelBullet(const FTransform& FireFromTransform, const float BulletVelocity);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE")
	TSubclassOf<UGameplayEffect> BulletDamageEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE")
	TObjectPtr<UFPCStaticMeshComponent> BulletMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FPEACE")
	TObjectPtr<UBoxComponent> BulletCollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE")
	TObjectPtr<UFPCProjectileMovementComponent> BulletMovementComp;

	virtual void OnPushedToPool_Implementation() override;
	virtual void OnPulledFromPool_Implementation() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	/*
	 * Whether the bullet has been fired yet or not
	 */
	bool HasbulletBeenFired = false;

	/*
	 * The time in seconds that has elapsed since the bullet was fired
	 */
	float CurrentBulletAge = 0;

	/*
	 * The time in seconds after which the bullet will be pooled
	 */
	const float BulletLifeSpan = 3;

	/*
	 * Reference to the character that is actively using the gun that fired this bullet
	 */
	UPROPERTY()
	TWeakObjectPtr<AFPCOperator> OwningOperator;

	/*
	 * Reference to the gun that fired this bullet
	 */
	UPROPERTY()
	TWeakObjectPtr<AFPCGun> OwningGun;

	UPROPERTY()
	TWeakObjectPtr<UFPCAbilitySystemComponent> OwningGunASC;

	void ToggleBulletActivation(const bool bActivate) const;

	void ApplyDamage(UAbilitySystemComponent* TargetAsc);
	
	UFUNCTION()
	void BulletOverlapedSomething(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
