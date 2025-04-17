// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RecoilHelper.generated.h"

class UCurveVector;
/**
 * Class that handles the recoil for a weapon.
 */
UCLASS()
class FPEACE_API URecoilHelper : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Default Constructor
	 */
	URecoilHelper();

	// **REQUIRED**
	UPROPERTY()
	UCurveVector* RecoilCurve;

	UPROPERTY()
	float FireRate;

	/*Optional variables to customize how fast the recoil resets and what is the max time 
	upto which the recovery can last */

	UPROPERTY()
	float RecoveryTime = 1.0f;

	UPROPERTY()
	float RecoilStrengthMultiplier = 1.0f;
	
	UPROPERTY()
	float RecoverySpeed = 10.0f;

	//Call this function when the firing begins, the recoil starts here
	void RecoilStart();

	//Called when firing stops
	void RecoilStop();

	//Needs to be called on event tick to update the control rotation.
	void RecoilTick(float DeltaTime);
	
private:


	UPROPERTY()
	bool bRecoil;
	UPROPERTY()
	bool Firing;
	UPROPERTY()
	bool bRecoilRecovery;

	//Timer Handles
	UPROPERTY()
	FTimerHandle FireTimer;
	UPROPERTY()
	FTimerHandle RecoveryTimer;
	
	//Rotators

	//Control rotation at the start of the recoil
	UPROPERTY()
	FRotator RecoilStartRot;
	
	//Control rotation change due to recoil
	FRotator RecoilDeltaRot;
	
	//Control rotation chnage due to player moving the mouse
	FRotator PlayerDeltaRot;
	
	//Temporary variable used in tick
	FRotator Del;

	//Player controller reference
	UPROPERTY()
	APlayerController* PCRef;

	//Automatically called in RecoilStart(), no need to call this explicitly
	UFUNCTION()
	void RecoilTimerFunction();

	//This function is automatically called, no need to call this. It's inside the Tick function
	UFUNCTION()
	void RecoveryStart();

	//This function too is automatically called from the recovery start function.
	UFUNCTION()
	void RecoveryTimerFunction();
};
