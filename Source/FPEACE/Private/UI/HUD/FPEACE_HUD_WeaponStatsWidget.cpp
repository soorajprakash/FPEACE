// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPEACE_HUD_WeaponStatsWidget.h"

#include "Gameplay/Actor/Operator/FPCOperator.h"
#include "Gameplay/Actor/Operator/Components/FPCCharacterWeaponManagerComponent.h"
#include "Gameplay/Weapon/FPCGun.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MVVMs/MVVM_HUD_WeaponStats.h"
#include "View/MVVMView.h"

void UFPEACE_HUD_WeaponStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!OwningOperator.IsValid())
		OwningOperator = Cast<AFPCOperator>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!OwningCharacterWeaponManagerComp.IsValid() && OwningOperator.IsValid())
		OwningCharacterWeaponManagerComp = OwningOperator->GetFPCCharacterWeaponManager();

	// Get the generated view model instance for this widget
	// The View model added in the WBP is of "Create Instance" type
	// So the instance is automatically generated after pre-construct
	if (UMVVMView* mvvmView = GetExtension<UMVVMView>())
		if (TScriptInterface<INotifyFieldValueChanged> VM = mvvmView->GetViewModel(ViewModelName))
			WidgetViewModelInstance = Cast<UMVVM_HUD_WeaponStats>(VM.GetObject());

	// Bind to player events
	if (OwningCharacterWeaponManagerComp.IsValid())
		OwningCharacterWeaponManagerComp.Get()->OnNewWeaponEquipped.AddDynamic(this, &UFPEACE_HUD_WeaponStatsWidget::OnNewWeaponEquipped);
}

void UFPEACE_HUD_WeaponStatsWidget::OnNewWeaponEquippedEvent_Implementation(AFPCGun* NewGun)
{
}

void UFPEACE_HUD_WeaponStatsWidget::OnNewWeaponEquipped(AFPCWeapon* SpawnedFPSWeaponRef, AFPCWeapon* SpawnedTPSWeaponRef)
{
	if (AFPCGun* NewGun = Cast<AFPCGun>(SpawnedTPSWeaponRef))
	{
		WidgetViewModelInstance->SetCurrentWepon(NewGun);
		NewGun->OnRemainingBulletsChanged.RemoveAll(this);
		NewGun->OnRemainingBulletsChanged.AddDynamic(WidgetViewModelInstance, &UMVVM_HUD_WeaponStats::SetRemainingBullets);
		OnNewWeaponEquippedEvent(NewGun);
	}
}
