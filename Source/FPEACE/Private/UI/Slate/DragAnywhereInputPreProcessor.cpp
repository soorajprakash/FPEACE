// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

// DragAnywhereInputPreProcessor.cpp
#include "DragAnywhereInputPreProcessor.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Gameplay/Actor/Operator/Components/FPCFullScreenJoystickComponent.h"

FVector2D FDragAnywhereInputPreProcessor::GetViewportSize() const
{
	if (UGameViewportClient* GVC = GEngine ? GEngine->GameViewport : nullptr)
	{
		FVector2D P;
		GVC->GetViewportSize(P);
		return P;
	}
	return FVector2D(1920, 1080);
}

bool FDragAnywhereInputPreProcessor::HandleMouseButtonDownEvent(FSlateApplication&, const FPointerEvent& E)
{
	if (!Comp.IsValid()) return false;
	const bool bLeftHalf = IsLeftHalf(GetViewportSize(), E.GetScreenSpacePosition());

	if (bLeftHalf && LeftId == INDEX_NONE)
	{
		LeftId = E.GetPointerIndex();
		Comp->_PP_TouchBegin(true, E.GetScreenSpacePosition());
	}
	else if (!bLeftHalf && RightId == INDEX_NONE)
	{
		RightId = E.GetPointerIndex();
		Comp->_PP_TouchBegin(false, E.GetScreenSpacePosition());
	}
	return false; // don't consume—let UMG receive it too
}

bool FDragAnywhereInputPreProcessor::HandleMouseMoveEvent(FSlateApplication&, const FPointerEvent& E)
{
	if (!Comp.IsValid()) return false;
	if (E.GetPointerIndex() == LeftId) Comp->_PP_TouchMove(true, E.GetScreenSpacePosition());
	if (E.GetPointerIndex() == RightId) Comp->_PP_TouchMove(false, E.GetScreenSpacePosition());
	return false;
}

bool FDragAnywhereInputPreProcessor::HandleMouseButtonUpEvent(FSlateApplication&, const FPointerEvent& E)
{
	if (!Comp.IsValid()) return false;
	if (E.GetPointerIndex() == LeftId)
	{
		LeftId = INDEX_NONE;
		Comp->_PP_TouchEnd(true);
	}
	if (E.GetPointerIndex() == RightId)
	{
		RightId = INDEX_NONE;
		Comp->_PP_TouchEnd(false);
	}
	return false;
}
