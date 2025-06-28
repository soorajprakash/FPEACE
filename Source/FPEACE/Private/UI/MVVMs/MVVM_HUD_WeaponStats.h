// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_HUD_WeaponStats.generated.h"

class AFPCGun;

/**
 * The View Model base class that holds the data for the weapon stats widget in the HUD
 */
UCLASS()
class UMVVM_HUD_WeaponStats : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetMagInfoText() const
	{
		FString Str = FString::Printf(TEXT("%d | %d"), RemainingBullets, MagCapacity);
		return FText::FromString(Str);
	}

	//---------------------- SETTERS ----------------------

	void SetCurrentWepon(const TWeakObjectPtr<AFPCGun>& InCurrentWepon);
	void SetMagCapacity(int32 InMagCapacity);

	UFUNCTION()
	void SetRemainingBullets(int32 InRemainingBullets);
	void SetTotalBullets(int32 InTotalBullets);

protected:
	UPROPERTY(BlueprintReadOnly, FieldNotify)
	TWeakObjectPtr<AFPCGun> CurrentGun;

	UPROPERTY(BlueprintReadOnly, FieldNotify)
	int MagCapacity;

	UPROPERTY(BlueprintReadOnly, FieldNotify)
	int RemainingBullets;

	UPROPERTY(BlueprintReadOnly, FieldNotify)
	int TotalBullets;
};
