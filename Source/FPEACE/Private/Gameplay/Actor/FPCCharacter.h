// Copyright © Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "FPCCharacter.generated.h"

class UFPCOperatorData;
class UFPCAbilitySystemComponent;
class AFPCGameplayPlayerController;
class UFPCSkeletalMeshComponent;
class UObjectPool;

UCLASS()
class FPEACE_API AFPCCharacter : public ACharacter, public IAbilitySystemInterface
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
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE")
	TArray<TSubclassOf<class UFPCOperatorAbilityBase>> BasicAbilities;

	//	--------------------- COMPONENTS ---------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPEACE|Components")
	TObjectPtr<UFPCSkeletalMeshComponent> MainBodyMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FPEACE|Components")
	TObjectPtr<UFPCAbilitySystemComponent> FPCAbilitySystemComponent;


	//	--------------------- OVERRIDES ---------------------

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void OnConstruction(const FTransform& Transform) override;
};
