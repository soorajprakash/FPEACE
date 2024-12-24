/**
 * InstaLODMeshReductionPCH.h (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODMeshReductionPCH.h
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */

#ifndef InstaLOD_InstaLODMeshReductionPCH_h
#define InstaLOD_InstaLODMeshReductionPCH_h

#include "UnrealEd.h"
#include "CoreTypes.h"

#include "InstaLOD/InstaLODAPI.h"
#include "InstaLOD/InstaLOD.h"

class InstaLODShared
{
public:

	/**< Called on authorization. */
	static void OpenAuthorizationWindowModal();

	/**< Called on deauthorization. */
	static void OpenDeauthorizationWindowModal();

	/**
	 * Method for opening the floating license unavailable window.
	 *
	 * @param ForceLicenseRefreshCallback callback function to be called to refresh the license.
	 */
	static void OpenFloatingLicenseUnavialableWindowModal(InstaLOD::IInstaLOD::pfnForceLicenseRefreshCallback ForceLicenseRefreshCallback);

	static FString Version;								/**< InstaLOD plugin version. */
	static FString LicenseInformation;					/**< The license information. */
	static const FString TargetedUnrealEngineVersion;	/**< The targted unreal engine version. */
};

DEFINE_LOG_CATEGORY_STATIC(LogInstaLOD, Log, All);

#endif
