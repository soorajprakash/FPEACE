// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "FPCUMGBlueprintFunctionLibrary.generated.h"

class UCanvasPanelSlot;

USTRUCT(BlueprintType)
struct FSlateTransformParams
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Center = FVector2D::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D VisualSize = FVector2D::ZeroVector;
};

UCLASS()
class UFPCUMGBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void MimicTouchInterfaceLayout(
		const FVector2D ViewportSizePx,
		const float DPIScale,
		UCanvasPanelSlot* CanvasSlot,
		const FSlateTransformParams& Spec);
};
