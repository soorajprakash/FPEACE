// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "FPCGunAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class FPEACE_API UFPCGunAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS_BASIC(UFPCGunAttributeSet, BaseBulletDamage)
	ATTRIBUTE_ACCESSORS_BASIC(UFPCGunAttributeSet, RemainingBulletsInMag)
	ATTRIBUTE_ACCESSORS_BASIC(UFPCGunAttributeSet, MagCapacity)
	ATTRIBUTE_ACCESSORS_BASIC(UFPCGunAttributeSet, RemainingMags)
	ATTRIBUTE_ACCESSORS_BASIC(UFPCGunAttributeSet, FireRate)
	ATTRIBUTE_ACCESSORS_BASIC(UFPCGunAttributeSet, ReloadTime)
	ATTRIBUTE_ACCESSORS_BASIC(UFPCGunAttributeSet, ReloadTime_Empty)
	ATTRIBUTE_ACCESSORS_BASIC(UFPCGunAttributeSet, BulletSpreadAngle)
	ATTRIBUTE_ACCESSORS_BASIC(UFPCGunAttributeSet, BulletSpreadAngle_ADS)
	ATTRIBUTE_ACCESSORS_BASIC(UFPCGunAttributeSet, BulletVelocity)

protected:
	/*
	 * The amount of damage dealt to the target character by each bullet of this gun
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE|Attributes")
	FGameplayAttributeData BaseBulletDamage;

	/*
	 * Remaining number of bullets in the mag
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE|Attributes")
	FGameplayAttributeData RemainingBulletsInMag;

	/*
	 * Number of bullets the magazine can hold.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE|Attributes")
	FGameplayAttributeData MagCapacity;

	/*
	 * The number of magazines the player currently holds for this weapon
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE|Attributes")
	FGameplayAttributeData RemainingMags;

	/*
	 * Number of bullets per second
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE|Attributes")
	FGameplayAttributeData FireRate;

	/*
	 * The time taken to reload the gun when the mag is not empty
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE|Attributes")
	FGameplayAttributeData ReloadTime;

	/*
	 * The time taken to reload the gun when the mag is empty
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE|Attributes")
	FGameplayAttributeData ReloadTime_Empty;

	/*
	 * The angle of the bullet spread cone in degrees of this gun when not aiming down sight
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE|Attributes")
	FGameplayAttributeData BulletSpreadAngle;

	/*
	 * The angle of the bullet spread cone in degrees of this gun when aiming down sight
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE|Attributes")
	FGameplayAttributeData BulletSpreadAngle_ADS;

	/*
	 * The velocity of the fired bullet in centimeters per second.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPEACE|Attributes")
	FGameplayAttributeData BulletVelocity;
};
