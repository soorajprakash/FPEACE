// Copyright © 2025 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCCharacterMovementComponent.h"

#include "Gameplay/Actor/FPCCharacter.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UFPCCharacterMovementComponent::UFPCCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UFPCCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwningCharacter = Cast<AFPCCharacter>(GetOwner());
}

void UFPCCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Fetch movement data
	const FVector& CharacterAcceleration = GetCurrentAcceleration();

	CharacterAcceleration2D = FVector(CharacterAcceleration.X, CharacterAcceleration.Y, 0);
	CharacterVelocity2D = FVector(Velocity.X, Velocity.Y, 0);
	RelativeVelocity = OwningCharacter->GetActorRotation().UnrotateVector(Velocity);
	CharacterVelocityZ = Velocity.Z;
	CharacterAbsoluteSpeed = Velocity.Length();
	CharacterAbsoluteSpeed2D = UKismetMathLibrary::VSizeXY(CharacterVelocity2D);
	CharacterNormalizedSpeed = CharacterAbsoluteSpeed / MaxWalkSpeed;

	// Update if the character is moving
	if (IsCharacterMoving && CharacterVelocity2D.Length() < 1)
		IsCharacterMoving = false;
	else if (!IsCharacterMoving && CharacterVelocity2D.Length() > 10)
		IsCharacterMoving = true;

	IsCharacterAccelerating = !CharacterAcceleration.IsNearlyZero();

	// Precompute normalized velocity and acceleration for clarity
	const FVector NormalizedVelocity = CharacterVelocity2D.GetSafeNormal();
	const FVector NormalizedAcceleration = CharacterAcceleration2D.GetSafeNormal();

	// Calculate the dot product once
	CurrentVelocityAccelerationDot = FVector::DotProduct(NormalizedVelocity, NormalizedAcceleration);
}
