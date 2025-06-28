// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPEACE_HUD_PlayerStatsWidget.h"
#include "Gameplay/ExtendedClasses/FPCGameInstance.h"
#include "ModelViewViewModel/Public/View/MVVMView.h"
#include "UI/MVVMs/MVVM_HUD_PlayerStats.h"

void UFPEACE_HUD_PlayerStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!FPCGameInstance.IsValid())
		if (UWorld* World = GetWorld())
			FPCGameInstance = Cast<UFPCGameInstance>(World->GetGameInstance());

	// Get the generated view model instance for this widget
	// The View model added in the WBP is of "Create Instance" type
	// So the instance is automatically generated after pre-construct
	if (UMVVMView* mvvmView = GetExtension<UMVVMView>())
		if (TScriptInterface<INotifyFieldValueChanged> VM = mvvmView->GetViewModel(ViewModelName))
			WidgetViewModelInstance = Cast<UMVVM_HUD_PlayerStats>(VM.GetObject());

	if (WidgetViewModelInstance)
	{
		// Bind View Model callbacks
		WidgetViewModelInstance->OnOperatorDataChangedEvent.BindLambda([this] { OnOperatorDataChanged(); });

		// Setup initial values for the VM (The data needed by this widget from the VM when initialized)
		// Set the current Operator Data from game instance
		WidgetViewModelInstance->SetSelectedOperatorData(FPCGameInstance->SelectedOperatorData);
	}
}

void UFPEACE_HUD_PlayerStatsWidget::OnOperatorDataChanged_Implementation()
{
}
