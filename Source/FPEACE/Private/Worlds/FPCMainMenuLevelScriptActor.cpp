// Copyright © 2024 Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPCMainMenuLevelScriptActor.h"
#include "CineCameraActor.h"
#include "CineCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenu/MenuOperatorLevelStage.h"


// Sets default values
AFPCMainMenuLevelScriptActor::AFPCMainMenuLevelScriptActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AFPCMainMenuLevelScriptActor::SetMenu3DCameraTarget(AActor* Target) const
{
	if (Menu3DCameraLevelActor)
		Menu3DCameraLevelActor->LookatTrackingSettings.ActorToTrack = Target;
}

void AFPCMainMenuLevelScriptActor::ToggleOperatorStage(bool On) const
{
	if (SpawnedOperatorStage)
		SpawnedOperatorStage->SetActorHiddenInGame(!On);
}

void AFPCMainMenuLevelScriptActor::SetMenu3DCameraPostProcessTint(FLinearColor Tint) const
{
	if (Menu3DCameraOperatorPostProcessMaterial)
		Menu3DCameraOperatorPostProcessMaterial->SetVectorParameterValue("TintColor", Tint);
}

void AFPCMainMenuLevelScriptActor::BeginPlay()
{
	if (!SpawnedOperatorStage)
	{
		SpawnedOperatorStage = GetWorld()->SpawnActor<AMenuOperatorLevelStage>(OperatorStageRef.LoadSynchronous());
		ToggleOperatorStage(true);
		SetMenu3DCameraTarget(SpawnedOperatorStage);
	}

	if (Menu3DCameraLevelActor)
	{
		UMaterialInterface* BaseMat = Cast<UMaterialInterface>(Menu3DCameraLevelActor->GetCameraComponent()->PostProcessSettings.WeightedBlendables.Array[0].Object);
		Menu3DCameraOperatorPostProcessMaterial = UMaterialInstanceDynamic::Create(BaseMat, this);
		Menu3DCameraLevelActor->GetCameraComponent()->PostProcessSettings.WeightedBlendables.Array[0].Object = Menu3DCameraOperatorPostProcessMaterial;
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			FViewTargetTransitionParams TransitionParams;
			PC->SetViewTarget(Menu3DCameraLevelActor, TransitionParams);
		}
	}

	Super::BeginPlay();
}
