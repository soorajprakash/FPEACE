// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "Gameplay/ExtendedClasses/Components/FPCActorComponent.h"
#include "FPCFullScreenJoystickComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeftAxisTick, FVector2D, Axis01); // [-1..1] with |Axis| in [0..1]
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRightDeltaEvent, FVector2D, DeltaPx);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleEvent);

USTRUCT(BlueprintType)
struct FDragMoveParams
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeadZonePx = 12.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRadiusPx = 160.f; // distance from origin for Mag=1
};

UCLASS(ClassGroup=(FPEACE), meta=(BlueprintSpawnableComponent))
class FPEACE_API UFPCFullScreenJoystickComponent : public UFPCActorComponent
{
	GENERATED_BODY()

public:
	UFPCFullScreenJoystickComponent();

	UFUNCTION(BlueprintCallable)
	void SetDragAnywhereEnabled(bool bEnable);

	// Runtime tuning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FPEACE|Input")
	FDragMoveParams MoveParams;

	// Events you’ll bind in your Character
	UPROPERTY(BlueprintAssignable, Category="FPEACE|Input")
	FLeftAxisTick OnLeftAxisTick;
	UPROPERTY(BlueprintAssignable, Category="FPEACE|Input")
	FSimpleEvent OnLeftEnded;
	UPROPERTY(BlueprintAssignable, Category="FPEACE|Input")
	FRightDeltaEvent OnRightDelta;
	UPROPERTY(BlueprintAssignable, Category="FPEACE|Input")
	FSimpleEvent OnRightEnded;

	// Called by the Slate overlay:
	void _OverlaySetLeftState(bool bHeld, const FVector2D& OriginScreenPx);
	void _OverlayUpdateLeftPos(const FVector2D& CurrentScreenPx);
	void _OverlayEndLeft();

	void _OverlaySetRightState(bool bHeld, const FVector2D& StartScreenPx);
	void _OverlayUpdateRightPos(const FVector2D& CurrentScreenPx);
	void _OverlayEndRight();

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float Dt, ELevelTick TickType, FActorComponentTickFunction* ThisTick) override;

private:
	TSharedPtr<class SDragAnywhereOverlay> Overlay;
	void RemoveOverlay();

	// Left state
	bool bLeftHeld = false;
	FVector2D LeftOriginPx = FVector2D::ZeroVector;
	FVector2D LeftCurrentPx = FVector2D::ZeroVector;

	// Right state (we forward deltas immediately via event)
	bool bRightHeld = false;
	FVector2D RightPrevPx = FVector2D::ZeroVector;
};
