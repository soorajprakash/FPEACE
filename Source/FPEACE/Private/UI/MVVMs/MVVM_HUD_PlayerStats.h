// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "DataStructures/FPEACEOperatorsDataMap.h"
#include "MVVM_HUD_PlayerStats.generated.h"


DECLARE_DELEGATE(FOnOperatorDataChangedDelegate);


/**
 * The View Model base class that holds the data for the Player stats widget in the HUD
 */
UCLASS()
class UMVVM_HUD_PlayerStats : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	
	FOnOperatorDataChangedDelegate OnOperatorDataChangedEvent;
	
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetHealthPercentage() const { return MaxHealth != 0 ? Health / MaxHealth : 0; }

	//---------------------- SETTERS ----------------------
	void SetScore(const int32& NewScore);
	void SetHealth(const float& NewHealth);
	void SetMaxHealth(const float& NewMaxHealth);
	void SetSelectedOperatorData(const FFPEACEOperatorData& NewOperatorData);

protected:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter)
	int32 Score = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter)
	float Health = 50;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter)
	float MaxHealth = 100;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter)
	FFPEACEOperatorData SelectedOperatorData;
};
