// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPEACE_CUI_HomeScreenWidget.h"
#include "Kismet/GameplayStatics.h"

void UFPEACE_CUI_HomeScreenWidget::StartPlay()
{
	// Open the sample level
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("PrototypeLevel"));
}
