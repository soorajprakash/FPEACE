/**
 * InstaLODRemeshTool.h (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODRemeshTool.h
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */

#pragma once

#include "CoreMinimal.h"
#include "Tools/InstaLODBakeBaseTool.h"
#include "InstaLODRemeshTool.generated.h"

UENUM()
enum class EInstaLODRemeshMode : uint8
{
	InstaLOD_Reconstruct		UMETA(DisplayName = "Reconstruct"),
	InstaLOD_Optimize			UMETA(DisplayName = "Optimize"),
	InstaLOD_ConvexHull			UMETA(DisplayName = "ConvexHull"),
	InstaLOD_UV					UMETA(DisplayName = "UV")
};

UENUM()
enum class EInstaLODRemeshFaceCount : uint8
{
	InstaLOD_Lowest				UMETA(DisplayName = "Lowest"),
	InstaLOD_Low				UMETA(DisplayName = "Low"),
	InstaLOD_Normal				UMETA(DisplayName = "Normal"),
	InstaLOD_High				UMETA(DisplayName = "High"),
	InstaLOD_Highest			UMETA(DisplayName = "Highest")
};

UENUM()
enum class EInstaLODRemeshResolution : uint8
{
	InstaLOD_Lowest				UMETA(DisplayName = "Lowest"),
	InstaLOD_Low				UMETA(DisplayName = "Low"),
	InstaLOD_Normal				UMETA(DisplayName = "Normal"),
	InstaLOD_High				UMETA(DisplayName = "High"),
	InstaLOD_VeryHigh			UMETA(DisplayName = "Very High"),
	InstaLOD_Extreme			UMETA(DisplayName = "Extreme")
};

UENUM()
enum class EInstaLODRemeshType : uint8
{
	InstaLOD_FuzzyFaceCountTarget	= 0		UMETA(DisplayName = "FuzzyFaceCountTarget"),
	InstaLOD_MaximumTriangles		= 1		UMETA(DisplayName = "MaximumTriangles"),
	InstaLOD_ScreenSizeInPixels		= 2		UMETA(DisplayName = "ScreenSizeInPixels")
};

UCLASS(Config=InstaLOD)
class INSTALODUI_API UInstaLODRemeshTool : public UInstaLODBakeBaseTool
{
	GENERATED_BODY()

	/// VARIABLES ///

	/************************************************************************/
	/* Settings																*/
	/************************************************************************/

public:

	/** Determines how the output surface will be constructed. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Mode"), Category = "Settings")
	EInstaLODRemeshMode RemeshMode = EInstaLODRemeshMode::InstaLOD_Reconstruct;

	/** The fuzzy face count target of the output geometry. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Fuzzy Face Count Target ", EditCondition = "RemeshMode != EInstaLODRemeshMode::InstaLOD_UV", EditConditionHides, RadioButton), Category = "Settings")
	EInstaLODRemeshFaceCount FuzzyFaceCountTarget = EInstaLODRemeshFaceCount::InstaLOD_Normal;

	/** The maximum amount of polygons for the target mesh. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Maximum Triangles", ClampMin = 0, RadioButton, EditCondition = "RemeshMode != EInstaLODRemeshMode::InstaLOD_UV", EditConditionHides), Category = "Settings")
	int32 MaximumTriangles = 0;

	/** Calculates the amount of polygons to remove based on the display size of the output model. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Screen Size In Pixels", UIMin = 1, UIMax = 2048, ClampMin = 1, ClampMax = 2048, RadioButton, EditCondition = "RemeshMode != EInstaLODRemeshMode::InstaLOD_UV", EditConditionHides), Category = "Settings")
	int32 ScreenSizeInPixels = 300;

	/** When 'Mode' is set to 'Reconstruct' and 'Screen Size in Pixels' is enabled, the pixel distance in screen size that will be merged together. This can be used to avoid the construction of unnecessary geometrical detail. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Pixel Merge Distance", ClampMin = 1, ClampMax = 32, EditCondition = "RemeshMode != EInstaLODRemeshMode::InstaLOD_UV", EditConditionHides), Category = "Settings")
	int32 PixelMergeDistance = 2;

	/** When 'Screen Size in Pixels' is enabled, automatically compute output texture dimensions based on screen size in pixels. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Automatic Texture Size", EditCondition = "RemeshMode != EInstaLODRemeshMode::InstaLOD_UV", EditConditionHides), Category = "Settings")
	bool bAutomaticTextureSize = false;

	/************************************************************************/
	/* Surface Construction													*/
	/************************************************************************/


	/** The resolution of the surface construction. The resolution is ^3 so memory and processing time grows exponentially. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Resolution", EditCondition = "RemeshMode != EInstaLODRemeshMode::InstaLOD_UV"), Category = "Surface Construction")
	EInstaLODRemeshResolution RemeshResolution = EInstaLODRemeshResolution::InstaLOD_Normal;

	/** Ignores backfaces during surface construction. NOTE: this can cause holes in the constructed geometry if face normals are not pointing outwards. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Ignore Backfaces", EditCondition = "RemeshMode != EInstaLODRemeshMode::InstaLOD_UV"), Category = "Surface Construction")
	bool bIgnoreBackfaces = true;

	/** When 'SurfaceMode' is 'Optimize', lock boundary vertices in place. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Lock Boundaries", EditCondition = "RemeshMode != EInstaLODRemeshMode::InstaLOD_UV"), Category = "Surface Construction")
	bool bLockBoundaries = false;

	/** When calculating normals for the output mesh, smooth faces if the normal angle is below this value (in degrees). */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Hard Angle Threshold", UIMin = 0.0f, UIMax = 180.0f, ClampMin = 0.0f, ClampMax = 180.0f, EditCondition = "RemeshMode != EInstaLODRemeshMode::InstaLOD_UV"), Category = "Surface Construction")
	float HardAngleThreshold = 70.0f;

	/** The welding distance can be used to weld holes in the input geometry. The welded mesh is only used for the surface construction is is not used during baking. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Welding Distance", UIMin = 0.0f, UIMax = 10.0f, EditCondition = "RemeshMode != EInstaLODRemeshMode::InstaLOD_UV"), Category = "Surface Construction")
	float WeldingDistance = 0.0f;

	/** The minimum distance between two UV shells in pixels. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Gutter Size In Pixels", ClampMin = 1, ClampMax = 64), Category = "UV")
	int32 GutterSizeInPixels = 5;

	/** Bake meshes do not influence the surface construction, but can be used to bake additional geometry onto the constructed surface. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Bake Mesh Suffix", Category = "Bake"))
	FString BakeMeshSuffix = TEXT("_bake");

	/** Factor to increase or decrease the automatically calculated ray length used during bake. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Auto Ray Length Factor", Category = "Bake"))
	float AutoRayLengthFactor = 1.0f;

	/** Backfaces will not occlude geometry. When disabled, backfaces will occlude geometry. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Ignore Backfaces", Category = "Bake"))
	bool bBakeIgnoreBackface = true;

	/** Use averaged face normals of the target mesh as ray directions. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Average Normals As Ray Direction", ClampMin = 0.1, ClampMax = 25, Category = "Bake"))
	bool bAverageNormalsAsRayDirection = false;

	/** Strategy used during unwrap. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Unwrap Strategy", Category = "UV"))
	EInstaLODUnwrapStrategy UnwrapStrategy = EInstaLODUnwrapStrategy::InstaLOD_Auto;
	
	/** The importance of UV space stretch during the unwrap. OFF: high levels of stretch allowed, Highest: no stretching allowed. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Unwrap Stretch Importance"), Category = "UV")
	EInstaLODImportance UnwrapStretchImportance = EInstaLODImportance::InstaLOD_Normal;

	/** [Preview] Performs a post-process to minimize the UV shell count. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Post-Process Layout"), Category = "UV")
	bool bPostProcessLayout = true;

	/** When 'UnwrapStrategy' is either 'HardSurfaceAxial' or 'HardSurfaceAngle', enables stitching of shells to reduce the amount of splits and shells. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Shell Stitching"), Category = "UV")
	bool bShellStitching = true;
	
	/** Insert UV shell splits for edges that do not share the same vertex normal. This improves the quality when rendering normal maps on normal split heavy geometry. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Insert Normal Splits"), Category = "UV")
	bool bInsertNormalSplits = false;

	/************************************************************************/
	/* Advanced																*/
	/************************************************************************/

	/** Makes the algorithm deterministic at the cost of speed. */
	UPROPERTY(Config, EditAnywhere, meta = (DisplayName = "Deterministic Execution"), Category = "Advanced")
	bool bDeterministic = false;

	/************************************************************************/
	/* Internal Use															*/
	/************************************************************************/

	/** Row Index for the Settings Rows. */
	UPROPERTY(Config)
	EInstaLODRemeshType SettingsCheckBox = EInstaLODRemeshType::InstaLOD_FuzzyFaceCountTarget;

	InstaLOD::IRemeshingOperation *Operation;
	InstaLOD::RemeshingResult OperationResult;

	/// FUNCTIONS ///

public:

	/** Constructor */
	UInstaLODRemeshTool();

	/** Returns current Remesh type from the selected Check Box. */
	EInstaLODRemeshType GetActiveSetting() const {
		return SettingsCheckBox;
	}

	/** Sets the current Remesh type setting. */	
	void SetActiveSetting(EInstaLODRemeshType NewType, bool bForceSave = true);

	/** Start - UInstaLODBaseTool Interface */
	virtual void OnMeshOperationExecute(bool bIsAsynchronous) override;
	virtual void DeallocMeshOperation() override;
	virtual bool IsMeshOperationSuccessful() const override;
	virtual InstaLOD::IInstaLODMaterial* GetBakeMaterial() override;
	virtual bool IsMaterialDataRequired() const override {
		return true;
	}
	virtual bool IsFreezingTransformsForMultiSelection() const override {
		return true;
	}
	virtual bool IsBakeMeshSupported() const override {
		return true;
	} 
	virtual FString GetBakeMeshSuffix() const
	{
		return BakeMeshSuffix;
	}
	virtual bool ReadSettingsFromJSONObject(const TSharedPtr<FJsonObject>& JsonObject) override;

	virtual FText GetFriendlyName() const override;
	virtual FText GetComboBoxItemName() const override;
	virtual FText GetOperationInformation() const override;
	virtual int32 GetOrderId() const override;
	virtual void ResetSettings() override;
	inline virtual bool IsGeneratingMultipleAssets() const override { return false; }
	/** End - UInstaLODBaseTool Interface */

private:

	InstaLOD::RemeshingSettings GetRemeshingSettings();
};
