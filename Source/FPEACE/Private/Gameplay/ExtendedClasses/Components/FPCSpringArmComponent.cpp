// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCSpringArmComponent.h"

void UFPCSpringArmComponent::SetArmLengthFromPitch(const UCurveFloat* PitchToArmLengthCurve, const FRotator& ControllerRotation)
{
	float SpringArmLength = PitchToArmLengthCurve->GetFloatValue(ControllerRotation.Pitch);
	TargetArmLength = SpringArmLength;
}
