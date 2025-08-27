// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once
#include "Framework/Application/IInputProcessor.h"

class UFPCFullScreenJoystickComponent;
class ICursor; // forward declare is fine

class FDragAnywhereInputPreProcessor : public IInputProcessor
{
public:
	explicit FDragAnywhereInputPreProcessor(UFPCFullScreenJoystickComponent* Owner) : Comp(Owner) {}

	// ***** required pure virtuals *****
	virtual const TCHAR* GetDebugName() const override { return TEXT("DragAnywhereInputPP"); }
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {}

	// pointer (mouse/touch) events
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& App, const FPointerEvent& E) override;
	virtual bool HandleMouseMoveEvent     (FSlateApplication& App, const FPointerEvent& E) override;
	virtual bool HandleMouseButtonUpEvent (FSlateApplication& App, const FPointerEvent& E) override;

private:
	TWeakObjectPtr<UFPCFullScreenJoystickComponent> Comp;
	int32 LeftId  = INDEX_NONE;
	int32 RightId = INDEX_NONE;

	FVector2D GetViewportSize() const;
	inline bool IsLeftHalf(const FVector2D& ViewSize, const FVector2D& ScreenPos) const
	{ return ScreenPos.X < ViewSize.X * 0.5f; }
};