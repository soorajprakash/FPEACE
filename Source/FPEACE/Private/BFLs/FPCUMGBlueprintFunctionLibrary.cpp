// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCUMGBlueprintFunctionLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

void UFPCUMGBlueprintFunctionLibrary::MimicTouchInterfaceLayout(
	const FVector2D ViewportSizePx,
	const float DPIScale,
	UCanvasPanelSlot* CanvasSlot,
	const FSlateTransformParams& Spec)
{
	if (!CanvasSlot) return;

	const float ScaleFactor = FMath::Max(ViewportSizePx.X, ViewportSizePx.Y) / (1024.f * FMath::Max(0.0001f, DPIScale));
	auto Resolve = [&](float P, float RelativeTo)-> float
	{
		if (P < -1.f) return RelativeTo + P * ScaleFactor;

		if (P < 0.f) return RelativeTo + P * RelativeTo;

		if (P <= 1.f) return P * RelativeTo;

		return P * ScaleFactor;
	};

	// Resolve to pixel space (joystick semantics)
	FVector2D CenterPx(
		Resolve(FMath::Abs(Spec.Center.X), ViewportSizePx.X) * FMath::Sign(Spec.Center.X),
		Resolve(FMath::Abs(Spec.Center.Y), ViewportSizePx.Y) * FMath::Sign(Spec.Center.Y)
	);
	FVector2D VisualSizePx(
		Resolve(Spec.VisualSize.X, ViewportSizePx.X),
		Resolve(Spec.VisualSize.Y, ViewportSizePx.Y)
	);

	const FVector2D Anchors = FVector2D(Spec.Center.X >= 0 ? 0 : 1, Spec.Center.Y >= 0 ? 0 : 1);

	CanvasSlot->SetAnchors(FAnchors(Anchors.X, Anchors.Y));
	CanvasSlot->SetAlignment(FVector2D::UnitVector * 0.5f);
	CanvasSlot->SetAutoSize(false);
	CanvasSlot->SetSize(VisualSizePx);
	CanvasSlot->SetPosition(CenterPx);
}
