// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MainMenu/FPCMenuOperator.h"
#include "FPEACEOperatorsDataMap.generated.h"

class AFPCCharacter;

USTRUCT(BlueprintType)
struct FFPEACEOperatorData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText OperatorName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AFPCCharacter> PlayableOperatorBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AFPCMenuOperator> MenuOperatorBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> OperatorCardImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor OperatorMenuColorScheme;

	// '==' operator for this struct
	bool operator==(const FFPEACEOperatorData& Other) const
	{
		return
			OperatorName.ToString() == Other.OperatorName.ToString() &&
			PlayableOperatorBP == Other.PlayableOperatorBP &&
			MenuOperatorBP == Other.MenuOperatorBP &&
			OperatorCardImage == Other.OperatorCardImage &&
			OperatorMenuColorScheme == Other.OperatorMenuColorScheme;
	}
};

/**
 * Holds the list of operators and their associated data in the game
 * Will be used by the Main menu and other levels to load/setup chosen character
 */
UCLASS(BlueprintType)
class FPEACE_API UFPEACEOperatorsDataMap : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, FFPEACEOperatorData> OperatorDataMap;
};
