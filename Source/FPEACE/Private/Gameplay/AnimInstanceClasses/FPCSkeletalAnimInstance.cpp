// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCSkeletalAnimInstance.h"

void UFPCSkeletalAnimInstance::RecordSequenceState(const FString& Key, FSkeletalAnimInstanceSequenceState& CurrentSequenceState)
{
	// Record the time stamp of the state
	CurrentSequenceState.TimeStamp = GetWorld()->GetTimeSeconds();
	SequenceStates.Add(Key, CurrentSequenceState);
}

bool UFPCSkeletalAnimInstance::GetSequenceState(const FString& MediaplayerID, FSkeletalAnimInstanceSequenceState& OutState) const
{
	if (const FSkeletalAnimInstanceSequenceState* State = SequenceStates.Find(MediaplayerID))
	{
		OutState = *State;
		return true;
	}

	return false;
}

void UFPCSkeletalAnimInstance::GetMostRecentSequenceState(FSkeletalAnimInstanceSequenceState& OutState) const
{
	float MostRecentTimeStamp = -FLT_MAX;
	for (const TPair<FString, FSkeletalAnimInstanceSequenceState>& Pair : SequenceStates)
	{
		if (Pair.Value.TimeStamp > MostRecentTimeStamp)
		{
			MostRecentTimeStamp = Pair.Value.TimeStamp;
			OutState = Pair.Value;
		}
	}
}

void UFPCSkeletalAnimInstance::GetMostRecentSequenceStateWithID(const TArray<FString>& IDSubstrings, FSkeletalAnimInstanceSequenceState& OutState)
{
	float MostRecentTimeStamp = -FLT_MAX;

	for (TPair<FString, FSkeletalAnimInstanceSequenceState>& Pair : SequenceStates)
	{
		bool HasAllSubstrings = true;
		for (const FString& Substring : IDSubstrings)
		{
			if (!Pair.Key.Contains(Substring))
			{
				HasAllSubstrings = false;
				break;
			}
		}

		if (HasAllSubstrings && Pair.Value.TimeStamp > MostRecentTimeStamp)
		{
			MostRecentTimeStamp = Pair.Value.TimeStamp;
			OutState = Pair.Value;
		}
	}
}
