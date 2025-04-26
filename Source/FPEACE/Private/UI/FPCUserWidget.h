// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPCUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPEACE_API UFPCUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Category = "FPEACE|UserWidget")
	bool bNotifyOnSizeChange = false;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetSizeChanged() const;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	                          const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
	FVector2D prevWidgetSize;

	void Test();
};
