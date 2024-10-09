// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCCharacter.h"
#include "FPCCapsuleComponent.h"
#include "FPCCharacterMovementComponent.h"
#include "FPCSkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FPCCameraComponent.h"
#include "FPCSpringArmComponent.h"
#include "InputMappingContext.h"

// Sets default values
AFPCCharacter::AFPCCharacter(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer
                                                                                 .SetDefaultSubobjectClass<UFPCSkeletalMeshComponent>(MeshComponentName)
                                                                                 .SetDefaultSubobjectClass<UFPCCharacterMovementComponent>(CharacterMovementComponentName)
                                                                                 .SetDefaultSubobjectClass<UFPCCapsuleComponent>(CapsuleComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Get references to the Extensions 
	FPCMeshComp = Cast<UFPCSkeletalMeshComponent>(GetMesh());
	FPCCapsuleComp = Cast<UFPCCapsuleComponent>(GetCapsuleComponent());

	// Create Components
	if (!FPCSpringArmComp)
	{
		FPCSpringArmComp = CreateDefaultSubobject<UFPCSpringArmComponent>(TEXT("FPCSpringArm"));
		FPCSpringArmComp->SetupAttachment(FPCCapsuleComp);
		FPCSpringArmComp->bUsePawnControlRotation = true;
	}

	if (!FPCCameraComp)
	{
		FPCCameraComp = CreateDefaultSubobject<UFPCCameraComponent>(TEXT("FPCCamera"));
		FPCCameraComp->SetupAttachment(FPCSpringArmComp);
	}
}

// Called when the game starts or when spawned
void AFPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get references to the Extensions 
	FPCMovementComp = Cast<UFPCCharacterMovementComponent>(GetMovementComponent());

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
			if (UEnhancedInputLocalPlayerSubsystem* EILPS = CastChecked<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()))
				if (!FPCCharacterInputMapping.IsNull())
					EILPS->AddMappingContext(FPCCharacterInputMapping.LoadSynchronous(), 1);
}

// Called to bind functionality to input
void AFPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
