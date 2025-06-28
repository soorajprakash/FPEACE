// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#include "FPCUserWidget.h"

void UFPCUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetWorld()->WorldType != EWorldType::EditorPreview)
	{
		FVector2D currentWidgetSize = GetCachedGeometry().GetLocalSize();
		if (prevWidgetSize != currentWidgetSize)
		{
			OnWidgetSizeChanged();
			prevWidgetSize = currentWidgetSize;
		}
	}
}

int32 UFPCUserWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                  const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
#if WITH_EDITOR
	if (GetWorld()->WorldType == EWorldType::EditorPreview)
		OnWidgetSizeChanged();
#endif

	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}
