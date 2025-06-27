// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/FPCUserWidget.h"
#include "FPEACE_HUD_WeaponStatsWidget.generated.h"

class AFPCOperator;
class AFPCWeapon;
class AFPCGun;
class UMVVM_HUD_WeaponStats;
class UFPCCharacterWeaponManagerComponent;

/**
 * The base class for the player stats widget
 * Mostly used in the HUD
 */
UCLASS()
class FPEACE_API UFPEACE_HUD_WeaponStatsWidget : public UFPCUserWidget
{
	GENERATED_BODY()

protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	FName ViewModelName = FName("WidgetVM");

	UPROPERTY()
	TObjectPtr<UMVVM_HUD_WeaponStats> WidgetViewModelInstance;

	UFUNCTION(BlueprintNativeEvent)
	void OnNewWeaponEquippedEvent(AFPCGun* NewGun);
	
private:

	UFUNCTION()
	void OnNewWeaponEquipped(AFPCWeapon* SpawnedFPSWeaponRef, AFPCWeapon* SpawnedTPSWeaponRef);
	
	UPROPERTY()
	TWeakObjectPtr<AFPCOperator> OwningOperator;
	
	UPROPERTY()
	TWeakObjectPtr<UFPCCharacterWeaponManagerComponent> OwningCharacterWeaponManagerComp;
};
