// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPCPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, Score);

/**
 * 
 */
UCLASS()
class FPEACE_API AFPCPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	FOnScoreChanged OnScoreChanged;

	UFUNCTION(BlueprintCallable)
	void AddScore(int32 Delta);

	void StopSurvivalTimer();

protected:
	UPROPERTY(VisibleAnywhere)
	float StartTimeSeconds = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SurvivalTimeSeconds = 0.f;

	virtual void BeginPlay() override;
};
