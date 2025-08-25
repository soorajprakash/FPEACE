// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "DragAnywhereOverlay.h"
#include "Gameplay/Actor/Operator/Components/FPCFullScreenJoystickComponent.h"

void SDragAnywhereOverlay::Construct(const FArguments& InArgs)
{
	Owner = InArgs._Owner;
	ChildSlot[SNullWidget::NullWidget];
}

bool SDragAnywhereOverlay::IsLeftHalf(const FGeometry& Geo, const FVector2D& ScreenPos) const
{
	const FVector2D L = Geo.AbsoluteToLocal(ScreenPos);
	return L.X < Geo.GetLocalSize().X * 0.5f;
}

FReply SDragAnywhereOverlay::OnTouchStarted(const FGeometry& Geo, const FPointerEvent& E)
{
	if (!Owner.IsValid()) return FReply::Unhandled();
	const FVector2D P = E.GetScreenSpacePosition();

	if (IsLeftHalf(Geo, P) && LeftId == INDEX_NONE)
	{
		LeftId = E.GetPointerIndex();
		Owner->_OverlaySetLeftState(true, P);
		return FReply::Handled().CaptureMouse(AsShared());
	}
	if (!IsLeftHalf(Geo, P) && RightId == INDEX_NONE)
	{
		RightId = E.GetPointerIndex();
		Owner->_OverlaySetRightState(true, P);
		return FReply::Handled().CaptureMouse(AsShared());
	}
	return FReply::Unhandled(); // extra touches go to UMG
}

FReply SDragAnywhereOverlay::OnTouchMoved(const FGeometry&, const FPointerEvent& E)
{
	if (!Owner.IsValid()) return FReply::Unhandled();
	const FVector2D P = E.GetScreenSpacePosition();

	if (E.GetPointerIndex() == LeftId)
	{
		Owner->_OverlayUpdateLeftPos(P);
		return FReply::Handled();
	}
	if (E.GetPointerIndex() == RightId)
	{
		Owner->_OverlayUpdateRightPos(P);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply SDragAnywhereOverlay::OnTouchEnded(const FGeometry&, const FPointerEvent& E)
{
	if (!Owner.IsValid()) return FReply::Unhandled();

	if (E.GetPointerIndex() == LeftId)
	{
		LeftId = INDEX_NONE;
		Owner->_OverlayEndLeft();
		return FReply::Handled().ReleaseMouseCapture();
	}
	if (E.GetPointerIndex() == RightId)
	{
		RightId = INDEX_NONE;
		Owner->_OverlayEndRight();
		return FReply::Handled().ReleaseMouseCapture();
	}
	return FReply::Unhandled();
}

// Mouse helper for desktop testing (one finger)
FReply SDragAnywhereOverlay::OnMouseButtonDown(const FGeometry& Geo, const FPointerEvent& M)
{
	if (!Owner.IsValid()) return FReply::Unhandled();
	const FVector2D P = M.GetScreenSpacePosition();

	// Treat mouse as right-hand look for convenience
	if (RightId == INDEX_NONE)
	{
		RightId = 0;
		Owner->_OverlaySetRightState(true, P);
		return FReply::Handled().CaptureMouse(AsShared());
	}
	return FReply::Unhandled();
}

FReply SDragAnywhereOverlay::OnMouseMove(const FGeometry&, const FPointerEvent& M)
{
	if (!Owner.IsValid()) return FReply::Unhandled();
	if (RightId != INDEX_NONE)
	{
		Owner->_OverlayUpdateRightPos(M.GetScreenSpacePosition());
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply SDragAnywhereOverlay::OnMouseButtonUp(const FGeometry&, const FPointerEvent&)
{
	if (!Owner.IsValid()) return FReply::Unhandled();
	if (RightId != INDEX_NONE)
	{
		RightId = INDEX_NONE;
		Owner->_OverlayEndRight();
		return FReply::Handled().ReleaseMouseCapture();
	}
	return FReply::Unhandled();
}
