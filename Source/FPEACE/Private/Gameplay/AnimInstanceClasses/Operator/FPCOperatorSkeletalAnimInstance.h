// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "FPCOperatorAnimInstance.h"
#include "FPCOperatorSkeletalAnimInstance.generated.h"

class UFPCOperatorLayerAnimInstance;


/*
 * Used to record the states of every sequence player or evaluators currently active in the anim instance
 */
USTRUCT(BlueprintType)
struct FFPCSequenceState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float CurrentPlayTime = 0;

	UPROPERTY(BlueprintReadWrite)
	float CurrentSequenceFraction = 0;

	float TimeStamp;
};


/**
 * The type of anim instance class used to go on a skeletal mesh component directly
 */
UCLASS()
class FPEACE_API UFPCOperatorSkeletalAnimInstance : public UFPCOperatorAnimInstance
{
	GENERATED_BODY()

public:
	/*
	 * This is used to only perform operations on the base anim instance for optimization
	 */
	UPROPERTY(BlueprintReadOnly)
	bool isBaseAnimInstance;

	/*
	 * Reference to the layer anim instance that is currently linked to this skeletal anim instance
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFPCOperatorLayerAnimInstance> CurrentLinkedAnimInstance;

	/*
	 * Used to hold the active weapon grip pose on the owning mesh
	 * Updated at runtime in blueprint
	 */
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAnimSequence> CurrentWeaponGripPose;

	/*
	 * Registers the sequence state of a player with a given ID
	 * If a ID doesn't exist, new one is created. If it does, the state is updated
	 */
	UFUNCTION(BlueprintCallable)
	void RecordSequenceState(const FString& Key, UPARAM(ref) FFPCSequenceState& CurrentSequenceState);

	/*
	 * Gets the sequence state of a player with a given ID
	 * Returns false if the ID doesn't exist
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	bool GetSequenceState(const FString& MediaplayerID, FFPCSequenceState& OutState) const;

	/*
	 * Get the sequence state of the most recently updated player. According to its own time stamp
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	void GetMostRecentSequenceState(FFPCSequenceState& OutState) const;

	/*
	 * Get the sequence state of the most recently updated player according to its own time stamp,
	 * but which has an ID that contains the given substrings
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	void GetMostRecentSequenceStateWithID(const TArray<FString>& IDSubstrings, FFPCSequenceState& OutState);
	
private:
	/*
	 * Holds the states of all registered sequence players or evaluators currently active in the anim instance
	 */
	TMap<FString, FFPCSequenceState> SequenceStates;
};
