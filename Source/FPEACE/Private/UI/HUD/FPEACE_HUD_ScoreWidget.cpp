// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPEACE_HUD_ScoreWidget.h"
#include "Gameplay/ExtendedClasses/FPCPlayerState.h"
#include "UI/MVVMs/MVVM_HUD_PlayerStats.h"
#include "View/MVVMView.h"

class UMVVMView;

void UFPEACE_HUD_ScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FPCPlayerState = GetOwningPlayer()->GetPlayerState<AFPCPlayerState>();
	if (FPCPlayerState.IsValid())
		FPCPlayerState->OnScoreChanged.AddDynamic(this, &UFPEACE_HUD_ScoreWidget::HandleScoreChanged);

	// Get the generated view model instance for this widget
	// The View model added in the WBP is of "Create Instance" type
	// So the instance is automatically generated after pre-construct
	if (UMVVMView* mvvmView = GetExtension<UMVVMView>())
		if (TScriptInterface<INotifyFieldValueChanged> VM = mvvmView->GetViewModel(ViewModelName))
			WidgetViewModelInstance = Cast<UMVVM_HUD_PlayerStats>(VM.GetObject());
}

void UFPEACE_HUD_ScoreWidget::OnScoreChanged_Implementation(int32 Score)
{
}

void UFPEACE_HUD_ScoreWidget::HandleScoreChanged(int32 Score)
{
	if (WidgetViewModelInstance)
		WidgetViewModelInstance->SetScore(Score);

	OnScoreChanged(Score);
}
