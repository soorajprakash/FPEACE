// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "UI/MVVMs/MVVM_HUD_WeaponStats.h"
#include "Gameplay/Weapon/FPCGun.h"


void UMVVM_HUD_WeaponStats::SetCurrentWepon(const TWeakObjectPtr<AFPCGun>& InCurrentWepon)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentGun, InCurrentWepon))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CurrentGun);
		SetMagCapacity(CurrentGun->GetGunSettings().MagCapacity);
		SetRemainingBullets(CurrentGun->GetCurrentRemainingBullets());
	}
}

void UMVVM_HUD_WeaponStats::SetMagCapacity(int32 InMagCapacity)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MagCapacity, InMagCapacity))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MagCapacity);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMagInfoText);
	}
}

void UMVVM_HUD_WeaponStats::SetRemainingBullets(int32 InRemainingBullets)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(RemainingBullets, InRemainingBullets))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(RemainingBullets);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMagInfoText);
	}
}

void UMVVM_HUD_WeaponStats::SetTotalBullets(int32 InTotalBullets)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(TotalBullets, InTotalBullets))
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(TotalBullets);
}
