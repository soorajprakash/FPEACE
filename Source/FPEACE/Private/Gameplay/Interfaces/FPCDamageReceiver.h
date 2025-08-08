// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPCDamageReceiver.generated.h"

class AFPCCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFPCDamageReceiver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPEACE_API IFPCDamageReceiver
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnReceivedDamage(TWeakObjectPtr<AFPCCharacter> From, FName HitBone) =0;
};
