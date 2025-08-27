// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPCFullScreenJoystickComponent.h"
#include "Framework/Application/SlateApplication.h"
#include "UI/Slate/DragAnywhereInputPreProcessor.h"

UFPCFullScreenJoystickComponent::UFPCFullScreenJoystickComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFPCFullScreenJoystickComponent::SetDragAnywhereEnabled(bool bEnable)
{
	if (bEnable)
	{
		if (!PP.IsValid())
		{
			PP = MakeShared<FDragAnywhereInputPreProcessor>(this);
			// Register so it runs BEFORE widgets; we still return false so UI also gets events
			FSlateApplication::Get().RegisterInputPreProcessor(PP); // priority defaults to high in practice
		}
		SetComponentTickEnabled(true);
	}
	else
	{
		if (PP.IsValid())
		{
			FSlateApplication::Get().UnregisterInputPreProcessor(PP.ToSharedRef());
			PP.Reset();
		}
		SetComponentTickEnabled(false);
		if (bLeftHeld) _PP_TouchEnd(true);
		if (bRightHeld) _PP_TouchEnd(false);
	}
}

void UFPCFullScreenJoystickComponent::EndPlay(const EEndPlayReason::Type R)
{
	SetDragAnywhereEnabled(false);
	Super::EndPlay(R);
}

void UFPCFullScreenJoystickComponent::TickComponent(float Dt, ELevelTick, FActorComponentTickFunction*)
{
	if (bLeftHeld)
	{
		const FVector2D d = LeftCurrPx - LeftOriginPx;
		const float r = d.Size();
		const float mag = FMath::Clamp((r - MoveParams.DeadZonePx) / FMath::Max(1.f, MoveParams.MaxRadiusPx), 0.f, 1.f);
		const FVector2D dir = r > KINDA_SMALL_NUMBER ? d / r : FVector2D::ZeroVector;
		OnLeftAxisTick.Broadcast(dir * mag); // fires EVERY TICK while held
	}
}

void UFPCFullScreenJoystickComponent::_PP_TouchBegin(bool bLeft, const FVector2D& P)
{
	if (bLeft)
	{
		bLeftHeld = true;
		LeftOriginPx = P;
		LeftCurrPx = P;
	}
	else
	{
		bRightHeld = true;
		RightPrevPx = P;
	}
}

void UFPCFullScreenJoystickComponent::_PP_TouchMove(bool bLeft, const FVector2D& P)
{
	if (bLeft && bLeftHeld) { LeftCurrPx = P; }
	else if (!bLeft && bRightHeld)
	{
		const FVector2D d = P - RightPrevPx;
		RightPrevPx = P;
		OnRightDelta.Broadcast(d);
	}
}

void UFPCFullScreenJoystickComponent::_PP_TouchEnd(bool bLeft)
{
	if (bLeft)
	{
		bLeftHeld = false;
		OnLeftAxisTick.Broadcast(FVector2D::ZeroVector);
		OnLeftEnded.Broadcast();
	}
	else
	{
		bRightHeld = false;
		OnRightDelta.Broadcast(FVector2D::ZeroVector);
		OnRightEnded.Broadcast();
	}
}
