// Copyright © 2024 Sooraj Prakash. All rights reserved.
// Unauthorized distribution of this file, or any part of it, is prohibited.


#include "FPCBullet.h"
#include "ObjectPoolSubsystem.h"
#include "Components/BoxComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCProjectileMovementComponent.h"
#include "Gameplay/ExtendedClasses/Components/FPCStaticMeshComponent.h"


// Sets default values
AFPCBullet::AFPCBullet()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!BulletCollisionComponent)
	{
		BulletCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HitDetectionVolume"));
		SetRootComponent(BulletCollisionComponent);
		BulletCollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
		BulletCollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
		BulletCollisionComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		BulletCollisionComponent->OnComponentBeginOverlap.RemoveAll(this);
		BulletCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPCBullet::BulletOverlapedSomething);
	}

	if (!BulletMeshComp)
	{
		BulletMeshComp = CreateDefaultSubobject<UFPCStaticMeshComponent>(TEXT("BulletMesh"));
		BulletMeshComp->SetupAttachment(RootComponent);
	}

	if (!BulletMovementComp)
	{
		BulletMovementComp = CreateDefaultSubobject<UFPCProjectileMovementComponent>(TEXT("BulletMovement"));
		BulletMovementComp->SetUpdatedComponent(RootComponent);
		BulletMovementComp->bRotationFollowsVelocity = true;
		BulletMovementComp->bRotationRemainsVertical = false;
		BulletMovementComp->ProjectileGravityScale = 1;
		BulletMovementComp->bSweepCollision = true;
	}
}

void AFPCBullet::SetOwners(AFPCCharacter* OwnerCharacter, AFPCGun* OwnerGun)
{
	OwningCharacter = OwnerCharacter;
	OwningGun = OwnerGun;
}

void AFPCBullet::PropelBullet(const FTransform& FireFromTransform, const float BulletVelocity)
{
	SetActorLocation(FireFromTransform.GetLocation());
	// Use the socket's Y axis as the desired forward direction.
	FVector DesiredForward = FireFromTransform.GetRotation().GetAxisY().GetSafeNormal();

	// Use the socket's Z axis as the desired up vector.
	FVector DesiredUp = FireFromTransform.GetRotation().GetAxisZ().GetSafeNormal();
	SetActorRotation(FRotationMatrix::MakeFromZX(DesiredForward, DesiredUp).Rotator());

	ToggleBulletActivation(true);

	if (BulletMovementComp)
	{
		BulletMovementComp->InitialSpeed = BulletVelocity * 0.5f;
		BulletMovementComp->MaxSpeed = BulletVelocity * 1.5f;
		BulletMovementComp->Velocity = GetActorUpVector() * BulletVelocity;
	}

	HasbulletBeenFired = true;
}

void AFPCBullet::OnPushedToPool_Implementation()
{
	Super::OnPushedToPool_Implementation();

	ToggleBulletActivation(false);

	HasbulletBeenFired = false;
	CurrentBulletAge = 0;
}

void AFPCBullet::OnPulledFromPool_Implementation()
{
	Super::OnPulledFromPool_Implementation();
}

void AFPCBullet::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasbulletBeenFired)
	{
		CurrentBulletAge += DeltaSeconds;

		if (CurrentBulletAge >= BulletLifeSpan)
			OwningCharacter->WorldObjectPool->Push(this);
	}
}

void AFPCBullet::ToggleBulletActivation(const bool bActivate) const
{
	// Bullet Mesh Comp
	BulletMeshComp->SetActive(bActivate);
	BulletMeshComp->SetVisibility(bActivate);
	BulletMeshComp->SetHiddenInGame(!bActivate);

	// Bullet Collision Comp
	BulletCollisionComponent->SetActive(bActivate);
	BulletCollisionComponent->SetUseCCD(bActivate);
	BulletCollisionComponent->SetCollisionEnabled(bActivate ? ECollisionEnabled::Type::QueryOnly : ECollisionEnabled::Type::NoCollision);
	BulletCollisionComponent->SetGenerateOverlapEvents(bActivate);

	// Bullet Movement Comp
	BulletMovementComp->SetActive(bActivate);
	BulletMovementComp->InitialSpeed = 0;
	BulletMovementComp->MaxSpeed = 0;
	BulletMovementComp->Velocity = FVector::ZeroVector;
}

void AFPCBullet::BulletOverlapedSomething(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
	DrawDebugSphere(GetWorld(), SweepResult.Location, 5, 12, FColor::Red, false, 2);
	OwningCharacter->WorldObjectPool->Push(this);
}
