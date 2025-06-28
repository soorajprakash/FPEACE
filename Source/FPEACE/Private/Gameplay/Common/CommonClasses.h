// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CommonClasses.generated.h"

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class UFPCDirectionalAnimSet : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> WeaponHandGripPose;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bAreDirectionalAnimsAdditive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bSyncUpperAndLowerBodyPlayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bSyncAsLeader;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> Forward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> Backward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> Right;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> Left;
};
