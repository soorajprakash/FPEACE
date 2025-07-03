// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPCCharacter.generated.h"

class UFPCAbilitySystemComponent;
class AFPCGameplayPlayerController;
class UFPCSkeletalMeshComponent;
class UObjectPool;

UCLASS()
class FPEACE_API AFPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//	--------------------- PUBLIC VARIABLES ---------------------
	UPROPERTY()
	TObjectPtr<UObjectPool> WorldObjectPool;


	//	--------------------- GETTER FUNCTIONS ---------------------

	TWeakObjectPtr<UFPCSkeletalMeshComponent> GetTPSBodyMeshComp() const;
	TWeakObjectPtr<AFPCGameplayPlayerController> GetFPCPlayerController() const;
	TWeakObjectPtr<UFPCAbilitySystemComponent> GetFPCAbilitySystemComp() const;

protected:
	//	--------------------- COMPONENTS ---------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCSkeletalMeshComponent> MainBodyMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FPEACE|Components")
	TObjectPtr<UFPCAbilitySystemComponent> FPCAbilitySystemComponent;

	/*
	 * Reference to the FPC Player Controller instance
	 */
	UPROPERTY(BlueprintReadOnly, Category="FPEACE")
	TObjectPtr<AFPCGameplayPlayerController> FPCPlayerControllerInstance;


	//	--------------------- OVERRIDES ---------------------

	/*
	 * When this character is possessed by the player controller
	 */
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnConstruction(const FTransform& Transform) override;
};
