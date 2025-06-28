// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.


#include "FPCOperatorComponentBase.h"
#include "Gameplay/AnimInstanceClasses/Operator/FPCOperatorSkeletalAnimInstance.h"
#include "Gameplay/Actor/Operator/FPCOperator.h"
#include "Gameplay/ExtendedClasses/Components/FPCSkeletalMeshComponent.h"


// Sets default values for this component's properties
UFPCOperatorComponentBase::UFPCOperatorComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UFPCOperatorComponentBase::BeginPlay()
{
	Super::BeginPlay();

	if (OwningOperator.IsValid())
		PlayerControllerRef = OwningOperator->GetFPCPlayerController();
}

void UFPCOperatorComponentBase::InitializeComponent()
{
	Super::InitializeComponent();

	if (OwningOperator == nullptr)
		OwningOperator = Cast<AFPCOperator>(GetOwner());

	if (OwningOperator.IsValid())
	{
		FPCOperatorMovementComp = OwningOperator->GetCharacterMovementComponent();
		FPCOperatorWeaponManagerComp = OwningOperator->GetFPCCharacterWeaponManager();
		FPCOperatorCameraManagerComp = OwningOperator->GetFPCCharacterCameraManager();
		FPCOperatorAnimationManagerComp = OwningOperator->GetFPCCharacterAnimationManager();
		FPCOperatorData = OwningOperator->GetCharacterData();

		FPCSpringArmComp = OwningOperator->GetFPCSpringArmComp();
		MainBodyMeshComp = OwningOperator->GetTPSBodyMeshComp();
		FPSBodyMeshComp = OwningOperator->GetFPSArmsMeshComp();
		FPSLowerBodyMeshComp = OwningOperator->GetFPSLowerBodyMeshComp();

		TPSMeshAnimInstance = Cast<UFPCOperatorSkeletalAnimInstance>(MainBodyMeshComp->GetAnimInstance());
		if (TPSMeshAnimInstance)
			TPSMeshAnimInstance->isBaseAnimInstance = true;
		FPSMeshAnimInstance = Cast<UFPCOperatorSkeletalAnimInstance>(FPSBodyMeshComp->GetAnimInstance());
	}
}
