// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPEACE_CUI_GameOverScreenWidget.h"

FText UFPEACE_CUI_GameOverScreenWidget::GetSurvivalTimeText(float Seconds) const
{
	const int32 totalSeconds = FMath::FloorToInt(Seconds);
	const int32 minutes = totalSeconds / 60;
	const int32 seconds = totalSeconds % 60;

	return FText::FromString(FString::Printf(TEXT("SURVIVED TIME - %02dm %02ds"), minutes, seconds));
}

FText UFPEACE_CUI_GameOverScreenWidget::GetScoreText(int32 Score) const
{
	return FText::FromString(FString::Printf(TEXT("SCORE - %02d"), Score));
}
