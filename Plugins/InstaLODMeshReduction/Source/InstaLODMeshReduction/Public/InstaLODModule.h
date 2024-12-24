/**
 * InstaLODModule.h (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODModule.h
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */
#ifndef InstaLOD_InstaLODModule_h
#define InstaLOD_InstaLODModule_h

#include "MeshUtilities.h"
#include "InstaLOD/InstaLODAPI.h"
#include "InstaLOD/InstaLOD.h"

/**
 * The EInstaLODMessageCategory enumeration represents InstaLOD message category.
 */
namespace EInstaLODMessageCategory
{
	enum Type : uint8
	{
		Warning = 0u,
		Error = 1u,
		Success = 2u,
		Info = 3u,
	};
}

class INSTALODMESHREDUCTION_API FInstaLODModule : public IMeshReductionModule
{
public:
	// IModuleInterface interface.
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual FString GetName() override
	{
		return TEXT("InstaLODMeshReduction");
	}
	virtual class IMeshMerging* GetDistributedMeshMergingInterface() override { return nullptr; }

	virtual IInstaLOD* GetInstaLODInterface();

	virtual class IMeshReduction* GetSkeletalMeshReductionInterface() override;
	virtual class IMeshReduction* GetStaticMeshReductionInterface() override;
	virtual class IMeshMerging* GetMeshMergingInterface() override;

	/** Returns the InstaLODAPI. */
	InstaLOD::IInstaLOD* GetInstaLODAPI() const { return InstaLODAPI; }

	/**
	 * Spawns a message dialog.
	 * 
	 * @param Type The button configuration type.
	 * @param Message The message.
	 * @param Title The title.
	 * @param MessageCategory The category of the message. Optional parameter defaulted to 'EAppMsgCategory::Warning'.
	 * @return The message dialog return value.
	 */
	static EAppReturnType::Type MessageDialogOpen(const EAppMsgType::Type Type, const FText& Message, const FText& Title, const EInstaLODMessageCategory::Type MessageCategory = EInstaLODMessageCategory::Warning);

	/** 
	 * Called by the SDK license verification thread when the license becomes unavailable. 
	 * 
	 * @param ForceLicenseRefreshCallback A delegate to be called to force refresh the license when the floating license becomes unavailable.
	 */
	static void LicenseUnavailableCallback(InstaLOD::IInstaLOD::pfnForceLicenseRefreshCallback ForceLicenseRefreshCallback);

	static InstaLOD::IInstaLOD::pfnForceLicenseRefreshCallback ForceLicenseRefreshDelegate;		/**< A delegate to be called to force refresh the license when the floating license becomes unavailable. */
	static bool bIsInstaLODFloatingLicenseAvailable;											/**< True if the floating license is available or the license is not a floating license, false otherwise. */
	static bool bAuthorizeInstaLODLater;														/**< Whether the user chose to authorize InstaLOD later when the floating license is not available. */
	static bool bAuthorizeWithAnotherAccount;													/**< Whether the user chose to authorize InstaLOD with another account. */

private:

	void InstallHooks();

	void InstallHooksLate();

	TArray< TSharedPtr< FString > > LODTypes;
	FDelegateHandle LateHooksDelegateHandle;
	InstaLOD::IInstaLOD* InstaLODAPI;
};

#endif
