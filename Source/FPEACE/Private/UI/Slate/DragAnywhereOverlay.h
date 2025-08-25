// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once
#include "Widgets/SCompoundWidget.h"

class UFPCFullScreenJoystickComponent;

class SDragAnywhereOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDragAnywhereOverlay)
		{
		}

		SLATE_ARGUMENT(TWeakObjectPtr<UFPCFullScreenJoystickComponent>, Owner)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual FReply OnTouchStarted(const FGeometry&, const FPointerEvent&) override;
	virtual FReply OnTouchMoved(const FGeometry&, const FPointerEvent&) override;
	virtual FReply OnTouchEnded(const FGeometry&, const FPointerEvent&) override;
	virtual FReply OnMouseButtonDown(const FGeometry&, const FPointerEvent&) override;
	virtual FReply OnMouseMove(const FGeometry&, const FPointerEvent&) override;
	virtual FReply OnMouseButtonUp(const FGeometry&, const FPointerEvent&) override;

private:
	TWeakObjectPtr<UFPCFullScreenJoystickComponent> Owner;
	int32 LeftId = INDEX_NONE;
	int32 RightId = INDEX_NONE;

	bool IsLeftHalf(const FGeometry& Geo, const FVector2D& ScreenPos) const;
};
