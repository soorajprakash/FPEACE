// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPCPlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class FPEACE_API AFPCPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/**
	 * Reference to the Input mapping context object
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPC")
	TSoftObjectPtr<UInputMappingContext> FPCCharacterInputMapping;

private:

	virtual void SetupInputComponent() override;
};
