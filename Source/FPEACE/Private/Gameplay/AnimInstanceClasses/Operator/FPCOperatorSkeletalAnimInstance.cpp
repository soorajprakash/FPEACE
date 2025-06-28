// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCOperatorSkeletalAnimInstance.h"

void UFPCOperatorSkeletalAnimInstance::RecordSequenceState(const FString& Key, FFPCSequenceState& CurrentSequenceState)
{
	//First wipe all the other state data
	for (TPair<FString, FFPCSequenceState>& DataPair : SequenceStates)
		if (DataPair.Key != Key)
			DataPair.Value = FFPCSequenceState();

	// Record the time stamp of the state
	CurrentSequenceState.TimeStamp = GetWorld()->GetTimeSeconds();

	if (SequenceStates.Contains(Key))
		SequenceStates[Key] = CurrentSequenceState;
	else
		SequenceStates.Add(Key, CurrentSequenceState);
}

bool UFPCOperatorSkeletalAnimInstance::GetSequenceState(const FString& MediaplayerID, FFPCSequenceState& OutState) const
{
	if (const FFPCSequenceState* State = SequenceStates.Find(MediaplayerID))
	{
		OutState = *State;
		return true;
	}

	return false;
}

void UFPCOperatorSkeletalAnimInstance::GetMostRecentSequenceState(FFPCSequenceState& OutState) const
{
	float MostRecentTimeStamp = -FLT_MAX;
	for (const TPair<FString, FFPCSequenceState>& Pair : SequenceStates)
	{
		if (Pair.Value.TimeStamp > MostRecentTimeStamp)
		{
			MostRecentTimeStamp = Pair.Value.TimeStamp;
			OutState = Pair.Value;
		}
	}
}

void UFPCOperatorSkeletalAnimInstance::GetMostRecentSequenceStateWithID(const TArray<FString>& IDSubstrings, FFPCSequenceState& OutState)
{
	float MostRecentTimeStamp = -FLT_MAX;

	for (TPair<FString, FFPCSequenceState>& Pair : SequenceStates)
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