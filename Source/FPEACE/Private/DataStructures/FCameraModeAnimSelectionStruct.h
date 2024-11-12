// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FCameraModeAnimSelectionStruct.generated.h"

enum class ECameraMode : uint8;
class UFPCAnimInstance;

USTRUCT(BlueprintType)
struct FPEACE_API FCameraModeAnimSelectionStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UFPCAnimInstance> FPSAnimationClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UFPCAnimInstance> TPSAnimationClass;
};
