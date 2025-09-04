// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "FPCMainMenuLevelScriptActor.generated.h"

class AMenuOperatorLevelStage;
class ACineCameraActor;

UCLASS()
class FPEACE_API AFPCMainMenuLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPCMainMenuLevelScriptActor();

	void SetMenu3DCameraTarget(AActor* Target) const;

	void ToggleOperatorStage(bool On) const;

	void SetMenu3DCameraPostProcessTint(FLinearColor Tint) const;

	UPROPERTY()
	TObjectPtr<AMenuOperatorLevelStage> SpawnedOperatorStage;

protected:
	UPROPERTY(EditDefaultsOnly, Category="FPEACE")
	TSoftClassPtr<AActor> OperatorStageRef;

	UPROPERTY(EditDefaultsOnly, Category="FPEACE")
	TObjectPtr<ACineCameraActor> Menu3DCameraLevelActor;

	virtual void BeginPlay() override;

	void SetupTemporalUpscalers();

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> Menu3DCameraOperatorPostProcessMaterial;
};
