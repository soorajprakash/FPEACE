// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCPlayerState.h"

void AFPCPlayerState::AddScore(int32 Delta)
{
	if (Delta <= 0)
		return;

	SetScore(GetScore() + Delta);
	OnScoreChanged.Broadcast(GetScore());
}

void AFPCPlayerState::StopSurvivalTimer()
{
	if (const UWorld* World = GetWorld())
		SurvivalTimeSeconds = FMath::Max(0.f, World->GetTimeSeconds() - StartTimeSeconds);
}

void AFPCPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (const UWorld* World = GetWorld())
		StartTimeSeconds = World->GetTimeSeconds();
}


