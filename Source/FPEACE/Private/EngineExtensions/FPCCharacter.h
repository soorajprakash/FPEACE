// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPCCharacter.generated.h"

class UFPCSpringArmComponent;
class UFPCCameraComponent;
class UInputMappingContext;
class UFPCCharacterMovementComponent;
class UFPCCapsuleComponent;
class UFPCSkeletalMeshComponent;

UCLASS()
class FPEACE_API AFPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCSkeletalMeshComponent> FPCMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCCapsuleComponent> FPCCapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCCharacterMovementComponent> FPCMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCCameraComponent> FPCCameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FPC/Components")
	TObjectPtr<UFPCSpringArmComponent> FPCSpringArmComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Setup Player Input
	 */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	/**
	 *Reference to Enhanced Input Component
	 */
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComp;

	/**
	 * Reference to the Input mapping context object
	 */
	TSoftObjectPtr<UInputMappingContext> FPCCharacterInputMapping;
};
