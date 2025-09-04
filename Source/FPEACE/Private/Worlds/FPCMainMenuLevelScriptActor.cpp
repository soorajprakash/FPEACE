// Copyright © Sooraj Prakash. All rights reserved.Unauthorized distribution or sharing of this code is prohibited.

#include "FPCMainMenuLevelScriptActor.h"
#include "CineCameraActor.h"
#include "CineCameraComponent.h"
#include "GameFramework/GameUserSettings.h"
#include "Interfaces/IPluginManager.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenu/MenuOperatorLevelStage.h"

#if WITH_DLSS
#include "DLSSLibrary.h"
#endif


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

	SetupTemporalUpscalers();
}

static bool SetCVarIntIfExists(const TCHAR* Name, int32 Value)
{
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name))
	{
		CVar->Set(Value, ECVF_SetByCode);
		UE_LOG(LogTemp, Warning, TEXT("[Upscaler] Set %s = %d"), Name, Value);
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("[Upscaler] CVar %s not found; skipping"), Name);
	return false;
}

static bool SetCVarFloatIfExists(const TCHAR* Name, float Value)
{
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name))
	{
		CVar->Set(Value, ECVF_SetByCode);
		UE_LOG(LogTemp, Warning, TEXT("[Upscaler] Set %s = %.1f"), Name, Value);
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("[Upscaler] CVar %s not found; skipping"), Name);
	return false;
}

void AFPCMainMenuLevelScriptActor::SetupTemporalUpscalers()
{
	UE_LOG(LogTemp, Warning, TEXT("HOLAAAAAAAAA 1"));
	// === Temporal Upscaler Selection ===
#if PLATFORM_ANDROID
	bool bASRAvailable = false;
	IConsoleVariable* asrCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ArmASR.Enable"));
	if (asrCVar)
	{
		// Arm ASR plugin is present. (We assume Vulkan SM5 is in use for temporal upscaling.)
		bASRAvailable = true;

		// Disable any other upscalers to avoid conflict (guarded)
		SetCVarIntIfExists(TEXT("r.Mobile.FSR.Enabled"), 0);
		SetCVarIntIfExists(TEXT("r.FidelityFX.FSR4.Enabled"), 0);
		SetCVarIntIfExists(TEXT("r.NGX.DLSS.Enable"), 0);

		// Enable Arm ASR (temporal upscaling on mobile)
		asrCVar->Set(1, ECVF_SetByCode); // asrCVar is known non-null here
		UE_LOG(LogTemp, Warning, TEXT("Arm ASR upscaling enabled as the primary upscaler on Android."));
	}
	if (!bASRAvailable)
	{
		// Arm ASR not available or not supported, use Mobile FSR as fallback (guarded)
		SetCVarIntIfExists(TEXT("r.ArmASR.Enable"), 0);
		SetCVarIntIfExists(TEXT("r.FidelityFX.FSR4.Enabled"), 0);
		SetCVarIntIfExists(TEXT("r.NGX.DLSS.Enable"), 0);

		if (SetCVarIntIfExists(TEXT("r.Mobile.FSR.Enabled"), 1))
		{
			UE_LOG(LogTemp, Warning, TEXT("Arm ASR not available - using Mobile FSR upscaling as a fallback on Android."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Arm ASR not available, and Mobile FSR plugin is missing - no upscaler activated on Android."));
		}
	}
#else  // Desktop (Windows/Linux)
	bool bDLSSActive = false;

	// Check if DLSS plugin is enabled (requires Projects module if you keep this)
	if (IPluginManager::Get().FindEnabledPlugin("DLSS"))
	{
		// Also call UDLSSLibrary::IsDLSSSupported() for runtime GPU check
		if (UDLSSLibrary::IsDLSSSupported())
		{
			// Disable other upscalers first (guarded)
			SetCVarIntIfExists(TEXT("r.FidelityFX.FSR4.Enabled"), 0);
			SetCVarIntIfExists(TEXT("r.ArmASR.Enable"), 0);
			SetCVarIntIfExists(TEXT("r.Mobile.FSR.Enabled"), 0);

			// Enable DLSS via NVIDIA’s library (preferred way)
			UDLSSLibrary::EnableDLSS(true); // turn on DLSS

			if (UDLSSLibrary::IsDLSSEnabled())
			{
				bDLSSActive = true;

				// Grab current output resolution (fallback to viewport if needed)
				FIntPoint OutputRes = FIntPoint::ZeroValue;
				if (UGameUserSettings* GUS = (GEngine ? GEngine->GetGameUserSettings() : nullptr))
					OutputRes = GUS->GetScreenResolution();

				if (OutputRes == FIntPoint::ZeroValue && GEngine && GEngine->GameViewport && GEngine->GameViewport->Viewport)
					OutputRes = GEngine->GameViewport->Viewport->GetSizeXY(); // <-- by ref (fixed)

				// Query DLSS for the *optimal* screen percentage for the chosen mode
				bool bModeSupported = false;
				bool bFixedScreenPercentage = false;
				float OptimalScreenPercentage = 0.0f;
				float MinScreenPercentage = 0.0f;
				float MaxScreenPercentage = 0.0f;
				float OptimalSharpnessIgnored = 0.0f; // DLSS sharpening is deprecated; we don’t use it.

				// If your SDK uses EDLSSQualityMode, switch the enum accordingly.
				UDLSSLibrary::GetDLSSModeInformation(
					UDLSSMode::UltraPerformance,
					OutputRes,
					/*out*/ bModeSupported,
					/*out*/ OptimalScreenPercentage,
					/*out*/ bFixedScreenPercentage,
					/*out*/ MinScreenPercentage,
					/*out*/ MaxScreenPercentage,
					/*out*/ OptimalSharpnessIgnored
				);

				// Guarded SP set
				SetCVarFloatIfExists(TEXT("r.ScreenPercentage"), OptimalScreenPercentage);

				UE_LOG(LogTemp, Warning, TEXT("NVIDIA DLSS 4 enabled as the primary upscaler on desktop (GPU supports DLSS)."));
			}
		}
	}
	if (!bDLSSActive)
	{
		// DLSS not available or not supported – use FSR 4 as fallback (guarded)
		SetCVarIntIfExists(TEXT("r.NGX.DLSS.Enable"), 0);
		SetCVarIntIfExists(TEXT("r.ArmASR.Enable"), 0);
		SetCVarIntIfExists(TEXT("r.Mobile.FSR.Enabled"), 0);

		if (SetCVarIntIfExists(TEXT("r.FidelityFX.FSR4.Enabled"), 1))
		{
			// (Optionally set FSR4.QualityMode or ScreenPercentage as needed)
			UE_LOG(LogTemp, Warning, TEXT("DLSS not available - using AMD FSR 4 upscaling as the fallback on desktop."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Neither DLSS nor FSR4 plugins are available - no upscaling solution activated on desktop."));
		}
	}
#endif
}
