// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPEACE_CUI_OperatorCardButton.h"
#include "DataStructures/FPEACEOperatorsDataMap.h"
#include "Gameplay/FPCGameInstance.h"
#include "MainMenu/MenuOperatorLevelStage.h"
#include "Worlds/FPCMainMenuLevelScriptActor.h"
#include  "FPEACE/Private/Gameplay/Character/FPCCharacter.h"

void UFPEACE_CUI_OperatorCardButton::SetupCard_Implementation(const FFPEACEOperatorData& OperatorData)
{
	CardOperatorData = OperatorData;
}

void UFPEACE_CUI_OperatorCardButton::NativeOnSelected(bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);

	if (MainMenuLevelScriptActor && FPCGameInstance)
	{
		TSubclassOf<AActor> SelectedOperatorMenuClass = CardOperatorData.MenuOperatorBP.LoadSynchronous();
		TSubclassOf<APawn> SelectedOperatorGameplayClass = CardOperatorData.PlayableOperatorBP.LoadSynchronous();

		// Set the selected operator class in the MainMenu
		MainMenuLevelScriptActor->SpawnedOperatorStage->SetOperatorActorClass(SelectedOperatorMenuClass);
		MainMenuLevelScriptActor->SetMenu3DCameraPostProcessTint(CardOperatorData.OperatorMenuColorScheme);

		// Set the selected operator class in the Game Instance
		FPCGameInstance->SelectedPawnClass = SelectedOperatorGameplayClass;
	}
}

void UFPEACE_CUI_OperatorCardButton::NativeOnDeselected(bool bBroadcast)
{
	Super::NativeOnDeselected(bBroadcast);
}

void UFPEACE_CUI_OperatorCardButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!FPCGameInstance)
		if (UWorld* World = GetWorld())
			FPCGameInstance = Cast<UFPCGameInstance>(World->GetGameInstance());

	if (!MainMenuLevelScriptActor)
		MainMenuLevelScriptActor = Cast<AFPCMainMenuLevelScriptActor>(GetWorld()->GetLevelScriptActor());
}
