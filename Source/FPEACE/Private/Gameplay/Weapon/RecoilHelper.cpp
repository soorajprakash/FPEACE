// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "RecoilHelper.h"
#include "Curves/CurveVector.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

URecoilHelper::URecoilHelper()
{
	//Just setting the Player controller reference so that you won't have to do it again before calling the functions.
	PCRef = UGameplayStatics::GetPlayerController(this, 0);
}

void URecoilHelper::RecoilStart()
{
	if (RecoilCurve)
	{
		//Setting all rotators to default values

		PlayerDeltaRot = FRotator(0.0f, 0.0f, 0.0f);
		RecoilDeltaRot = FRotator(0.0f, 0.0f, 0.0f);
		Del = FRotator(0.0f, 0.0f, 0.0f);
		RecoilStartRot = PCRef->GetControlRotation();

		Firing = true;

		//Timer for the recoil: I have set it to 10s but dependeding how long it takes to empty the gun mag, you can increase the time.

		GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &URecoilHelper::RecoilTimerFunction, 10.0f, false);

		bRecoil = true;
		bRecoilRecovery = false;
	}
}

void URecoilHelper::RecoilTimerFunction()
{
	bRecoil = false;
	GetWorld()->GetTimerManager().PauseTimer(FireTimer);
}

void URecoilHelper::RecoilStop()
{
	Firing = false;
}

void URecoilHelper::RecoveryStart()
{
	if (PCRef->GetControlRotation().Pitch > RecoilStartRot.Pitch)
	{
		bRecoilRecovery = true;
		GetWorld()->GetTimerManager().SetTimer(RecoveryTimer, this, &URecoilHelper::RecoveryTimerFunction, RecoveryTime, false);
	}
}

void URecoilHelper::RecoveryTimerFunction()
{
	bRecoilRecovery = false;
}

void URecoilHelper::RecoilTick(float DeltaTime)
{
	float recoiltime;
	FVector RecoilVec;
	if (bRecoil)
	{
		//Calculation of control rotation to update 

		recoiltime = GetWorld()->GetTimerManager().GetTimerElapsed(FireTimer);
		RecoilVec = RecoilStrengthMultiplier * RecoilCurve->GetVectorValue(recoiltime);
		Del.Roll = 0;
		Del.Pitch = (RecoilVec.Y);
		Del.Yaw = (RecoilVec.Z);
		PlayerDeltaRot = PCRef->GetControlRotation() - RecoilStartRot - RecoilDeltaRot;
		PCRef->SetControlRotation(RecoilStartRot + PlayerDeltaRot + Del);
		RecoilDeltaRot = Del;

		//Conditionally start resetting the recoil

		if (!Firing)
		{
			if (recoiltime > FireRate)
			{
				GetWorld()->GetTimerManager().ClearTimer(FireTimer);
				RecoilStop();
				bRecoil = false;
				RecoveryStart();
			}
		}
	}
	else if (bRecoilRecovery)
	{
		//Recoil resetting
		FRotator tmprot = PCRef->GetControlRotation();
		if (tmprot.Pitch >= RecoilStartRot.Pitch)
		{
			PCRef->SetControlRotation(UKismetMathLibrary::RInterpTo(PCRef->GetControlRotation(), PCRef->GetControlRotation() - RecoilDeltaRot, DeltaTime, 10.0f));
			RecoilDeltaRot = RecoilDeltaRot + (PCRef->GetControlRotation() - tmprot);
		}
		else
		{
			RecoveryTimer.Invalidate();
		}
	}
}
