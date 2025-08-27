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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FPEACE|Input")
	FDragMoveParams MoveParams;

	UPROPERTY(BlueprintAssignable)
	FLeftAxisTick OnLeftAxisTick; // [-1..1], |.| in [0..1]
	UPROPERTY(BlueprintAssignable)
	FSimpleEvent OnLeftEnded;
	UPROPERTY(BlueprintAssignable)
	FRightDeltaEvent OnRightDelta;
	UPROPERTY(BlueprintAssignable)
	FSimpleEvent OnRightEnded;

	// called from the pre-processor:
	void _PP_TouchBegin(bool bLeft, const FVector2D& ScreenPos);
	void _PP_TouchMove(bool bLeft, const FVector2D& ScreenPos);
	void _PP_TouchEnd(bool bLeft);

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float Dt, ELevelTick, FActorComponentTickFunction*) override;

private:
	TSharedPtr<class FDragAnywhereInputPreProcessor> PP;

	bool bLeftHeld = false, bRightHeld = false;
	FVector2D LeftOriginPx = FVector2D::ZeroVector;
	FVector2D LeftCurrPx = FVector2D::ZeroVector;
	FVector2D RightPrevPx = FVector2D::ZeroVector;
};
