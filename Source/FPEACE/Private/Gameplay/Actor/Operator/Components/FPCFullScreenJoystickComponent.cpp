// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPCFullScreenJoystickComponent.h"
#include "Engine/GameViewportClient.h"
#include "UI/Slate/DragAnywhereOverlay.h"

UFPCFullScreenJoystickComponent::UFPCFullScreenJoystickComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFPCFullScreenJoystickComponent::SetDragAnywhereEnabled(bool bEnable)
{
	SetComponentTickEnabled(bEnable);
	if (bEnable)
	{
		if (!Overlay.IsValid())
		{
			if (UGameViewportClient* GVC = GetWorld() ? GetWorld()->GetGameViewport() : nullptr)
			{
				TWeakObjectPtr Self = this;
				TSharedRef<SDragAnywhereOverlay> Layer =
					SNew(SDragAnywhereOverlay).Owner(Self);
				Overlay = Layer;
				GVC->AddViewportWidgetContent(Layer, 1);
			}
		}
	}
	else
	{
		RemoveOverlay();
		bLeftHeld = bRightHeld = false;
	}
}

void UFPCFullScreenJoystickComponent::RemoveOverlay()
{
	if (Overlay.IsValid())
	{
		if (UGameViewportClient* GVC = GetWorld() ? GetWorld()->GetGameViewport() : nullptr)
			GVC->RemoveViewportWidgetContent(Overlay.ToSharedRef());
		Overlay.Reset();
	}
}

void UFPCFullScreenJoystickComponent::EndPlay(const EEndPlayReason::Type R)
{
	RemoveOverlay();
	Super::EndPlay(R);
}

void UFPCFullScreenJoystickComponent::TickComponent(float Dt, ELevelTick, FActorComponentTickFunction*)
{
	if (bLeftHeld)
	{
		const FVector2D d = LeftCurrentPx - LeftOriginPx;
		const float r = d.Size();
		const float mag = FMath::Clamp((r - MoveParams.DeadZonePx) / FMath::Max(1.f, MoveParams.MaxRadiusPx), 0.f, 1.f);
		FVector2D axis = r > KINDA_SMALL_NUMBER ? d / r : FVector2D::ZeroVector; // direction
		OnLeftAxisTick.Broadcast(axis * mag); // fires EVERY tick while held
	}
}

// ---- Overlay callbacks ----
void UFPCFullScreenJoystickComponent::_OverlaySetLeftState(bool bHeld, const FVector2D& OriginPx)
{
	bLeftHeld = bHeld;
	LeftOriginPx = OriginPx;
	LeftCurrentPx = OriginPx;
}

void UFPCFullScreenJoystickComponent::_OverlayUpdateLeftPos(const FVector2D& CurPx)
{
	LeftCurrentPx = CurPx;
}

void UFPCFullScreenJoystickComponent::_OverlayEndLeft()
{
	bLeftHeld = false;
	OnLeftAxisTick.Broadcast(FVector2D::ZeroVector);
	OnLeftEnded.Broadcast();
}

void UFPCFullScreenJoystickComponent::_OverlaySetRightState(bool bHeld, const FVector2D& StartPx)
{
	bRightHeld = bHeld;
	RightPrevPx = StartPx;
}

void UFPCFullScreenJoystickComponent::_OverlayUpdateRightPos(const FVector2D& CurPx)
{
	if (!bRightHeld) return;
	const FVector2D delta = CurPx - RightPrevPx;
	RightPrevPx = CurPx;
	OnRightDelta.Broadcast(delta); // emit immediate deltas for look
}

void UFPCFullScreenJoystickComponent::_OverlayEndRight()
{
	bRightHeld = false;
	OnRightDelta.Broadcast(FVector2D::ZeroVector);
	OnRightEnded.Broadcast();
}
