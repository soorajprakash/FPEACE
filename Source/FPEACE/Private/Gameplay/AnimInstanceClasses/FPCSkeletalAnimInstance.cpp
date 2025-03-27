// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCSkeletalAnimInstance.h"

#include "Gameplay/FPCSkeletalMeshComponent.h"
#include "Gameplay/Character/FPCCameraComponent.h"
#include "Gameplay/Character/FPCCharacter.h"
#include "Gameplay/Character/FPCCharacterCameraManagerComponent.h"
#include "Gameplay/Character/FPCCharacterMovementComponent.h"
#include "Gameplay/Character/FPCCharacterWeaponManagerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UFPCSkeletalAnimInstance::RecordSequenceState(const FString& Key, FSkeletalAnimInstanceSequenceState& CurrentSequenceState)
{
	//First wipe all the other state data
	for (TPair<FString, FSkeletalAnimInstanceSequenceState>& DataPair : SequenceStates)
		if (DataPair.Key != Key)
			DataPair.Value = FSkeletalAnimInstanceSequenceState();

	// Record the time stamp of the state
	CurrentSequenceState.TimeStamp = GetWorld()->GetTimeSeconds();

	if (SequenceStates.Contains(Key))
		SequenceStates[Key] = CurrentSequenceState;
	else
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