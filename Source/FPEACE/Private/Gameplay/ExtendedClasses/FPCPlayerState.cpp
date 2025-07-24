// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCPlayerState.h"

void AFPCPlayerState::AddScore(int32 Delta)
{
	if (Delta <= 0)
		return;

	SetScore(GetScore() + Delta);
	OnScoreChanged.Broadcast(GetScore());
}
