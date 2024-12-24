/**
 * InstaLODSettings.cpp (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODSettings.cpp
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */

#include "InstaLODSettings.h"
#include "InstaLODUIPCH.h"

#include "Interfaces/IPluginManager.h"

#include "InstaLODModule.h"
#include "InstaLODUIModule.h"
#include "Utilities/InstaLODUtilities.h"
#include "Slate/InstaLODWindow.h"

#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"

#define LOCTEXT_NAMESPACE "InstaLODUI"

UInstaLODSettings::UInstaLODSettings()
{
	FetchLicenseInformation();
}

/************************************************************************/
/* Utilities															*/
/************************************************************************/

void UInstaLODSettings::ClearLODsFromSelection()
{
	if (MeshComponents.Num() == 0)
		return;

	const FText MessageContents = FText::FromString("Do you really want to clear the selection of all LODs?\nThis will remove all LODs for all Instances of this Mesh and is not undoable.");
	const FText MessageTitle = LOCTEXT("InstaLODClearSelectionTitle", "InstaLOD: Clear Selection of all LODs?");

	if (FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, MessageContents, MessageTitle) == EAppReturnType::Cancel)
		return;

	for (TSharedPtr<FInstaLODMeshComponent> MeshComponent : MeshComponents)
	{
		if (MeshComponent.IsValid())
		{
			if (MeshComponent->StaticMeshComponent.IsValid())
			{
				UStaticMesh *const Mesh = MeshComponent->StaticMeshComponent->GetStaticMesh();

				if (Mesh == nullptr)
					continue;

				UInstaLODUtilities::RemoveAllLODsFromStaticMesh(Mesh);
				Mesh->SetLODGroup(NAME_None);
			}
			else if (MeshComponent->SkeletalMeshComponent.IsValid())
			{
				UInstaLODUtilities::RemoveAllLODsFromSkeletalMesh(MeshComponent->SkeletalMeshComponent->GetSkeletalMeshAsset());
			}
		}
	}
}

void UInstaLODSettings::AuthorizeWorkstation()
{
	if (AccountName.IsEmpty() || SerialPassword.IsEmpty())
	{
		const FText Title = NSLOCTEXT(LOCTEXT_NAMESPACE, "AuthorizeMessage_Title", "No Username or Password!");
		FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, NSLOCTEXT(LOCTEXT_NAMESPACE, "AuthorizeMessage_Message", "Please enter Username and Password!"), Title);
		return;
	}

	// callback to the InstaLODAPI
	FInstaLODModule& InstaLODModule = FModuleManager::LoadModuleChecked<FInstaLODModule>("InstaLODMeshReduction");

	if (InstaLODModule.GetInstaLODAPI() == nullptr)
	{
		const FText Title = NSLOCTEXT(LOCTEXT_NAMESPACE, "AuthorizeError_Title", "Couldn't retrieve InstaLOD Module.");
		FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, NSLOCTEXT(LOCTEXT_NAMESPACE, "AuthorizeError_Message", "Please make sure the InstaLOD Module loaded properly!"), Title);
		return;
	}

	if (InstaLODModule.GetInstaLODAPI()->AuthorizeMachine(TCHAR_TO_UTF8(*AccountName.ToString()), TCHAR_TO_UTF8(*SerialPassword.ToString())))
	{
		const FText MessageTitle = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODAuthorization", "InstaLOD: Machine Authorization");
		const FText AuthorizedMessageContent = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODAuthorizationCompleted", "A valid InstaLOD license has been acquired for this machine.\n\nInstaLOD will automatically refresh the license if necessary.");

		FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, AuthorizedMessageContent, MessageTitle);
		FetchLicenseInformation();

		SInstaLODWindow* const InstaLODWindowPtr = GetInstaLODWindow();
		check(InstaLODWindowPtr);

		// This will result in everything being refreshed
		InstaLODWindowPtr->UpdateWindow();
		return;
	}

	const FText Title = NSLOCTEXT(LOCTEXT_NAMESPACE, "AuthorizeError_Title", "Authorization Error");
	const FString AuthorizationInformation = UTF8_TO_TCHAR(InstaLODModule.GetInstaLODAPI()->GetAuthorizationInformation());

	FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, FText::FromString("Failed to authorize machine:\n\n" + AuthorizationInformation), Title);
}

void UInstaLODSettings::DeauthorizeWorkstation()
{
	if (AccountName.IsEmpty() || SerialPassword.IsEmpty())
	{
		FText Title = NSLOCTEXT(LOCTEXT_NAMESPACE, "DeauthorizeMessage_Title", "No Username or Password!");
		FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, NSLOCTEXT(LOCTEXT_NAMESPACE, "DeauthorizeMessage_Message", "Please enter Username and Password!"), Title);
		return;
	}

	// callback to the InstaLODAPI
	FInstaLODModule& InstaLODModule = FModuleManager::LoadModuleChecked<FInstaLODModule>("InstaLODMeshReduction");
	if (InstaLODModule.GetInstaLODAPI()->DeauthorizeMachine(TCHAR_TO_UTF8(*AccountName.ToString()), TCHAR_TO_UTF8(*SerialPassword.ToString())))
	{
		FetchLicenseInformation();

		SInstaLODWindow* const InstaLODWindowPtr = GetInstaLODWindow();
		check(InstaLODWindowPtr);
		// this will result in everything being refreshed
		InstaLODWindowPtr->UpdateWindow();
		return;
	}

	const FText Title = NSLOCTEXT(LOCTEXT_NAMESPACE, "DeauthorizeError_Title", "Deauthorization Error");
	const FString AuthorizationInformation = UTF8_TO_TCHAR(InstaLODModule.GetInstaLODAPI()->GetAuthorizationInformation());
	const FText Message = FText::Format(NSLOCTEXT(LOCTEXT_NAMESPACE,"DeauthorizeError_Message", "Failed to deauthorize machine:\n\n{0}"), FText::FromString(AuthorizationInformation));

	FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, Message, Title, EInstaLODMessageCategory::Error);
}

void UInstaLODSettings::ResetToolSettings()
{
	FInstaLODUIModule& InstaLODUIModule = FModuleManager::LoadModuleChecked<FInstaLODUIModule>("InstaLODUI");
	TArray<UInstaLODBaseTool*> Tools = InstaLODUIModule.GetAllTools();

	for (UInstaLODBaseTool* const Tool : Tools)
	{
		if (Tool == nullptr)
			continue;

		Tool->ResetSettings();
	}

	const FText Title = NSLOCTEXT(LOCTEXT_NAMESPACE, "SettingsReset_Title", "Settings Reset.");
	FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, NSLOCTEXT(LOCTEXT_NAMESPACE, "SettingsReset_Message", "All settings have been reset to their default values."), Title);
}

void UInstaLODSettings::LoadProfile()
{
	IDesktopPlatform* const Desktop = FDesktopPlatformModule::Get();
	if (Desktop == nullptr)
		return;

	const FString FileTypes = TEXT("(InstaLOD JSON Profile)|*.json");
	TArray<FString> OutFileNames;

	if (!Desktop->OpenFileDialog(nullptr, TEXT("Load InstaLOD Profile"), FString(), FString(), FileTypes, EFileDialogFlags::None, OutFileNames))
		return;

	check(OutFileNames.Num() == 1);

	const FString FilePath = OutFileNames[0];

	if (FilePath.Len() == 0)
		return;

	FString JsonContent;

	if (!FFileHelper::LoadFileToString(JsonContent, *FilePath))
	{
		UE_LOG(LogInstaLOD, Warning, TEXT("InstaLOD: Reading InstaLOD JSON Profile file failed."));
		return;
	}

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonContent);
	TSharedPtr<FJsonObject> Json = MakeShareable(new FJsonObject);

	if (!FJsonSerializer::Deserialize(JsonReader, Json, FJsonSerializer::EFlags::None))
	{
		UE_LOG(LogInstaLOD, Warning, TEXT("InstaLOD: JSON parsing failed."));
		return;
	}

	if (!Json.IsValid())
	{
		UE_LOG(LogInstaLOD, Warning, TEXT("InstaLOD: JSON parsing failed."));
		return;
	}

	if (!Json->HasField(TEXT("Operation")))
	{
		UE_LOG(LogInstaLOD, Warning, TEXT("InstaLOD: JSON parsing failed."));
		return;
	}
	else
	{
		FString OutOperation;
		if (!Json->TryGetStringField(TEXT("Operation"), OutOperation))
		{
			UE_LOG(LogInstaLOD, Warning, TEXT("InstaLOD: JSON parsing failed."));
			return;
		}
		if (OutOperation != TEXT("MeshOperation"))
		{
			UE_LOG(LogInstaLOD, Warning, TEXT("InstaLOD: JSON parsing failed."));
			return;
		}
	}

	TSharedPtr<FJsonValue> MeshOperationValue = Json->TryGetField(TEXT("MeshOperation"));

	if (!MeshOperationValue.IsValid())
	{
		UE_LOG(LogInstaLOD, Warning, TEXT("InstaLOD: JSON parsing failed."));
		return;
	}

	TSharedPtr<FJsonObject> MeshOperation = MeshOperationValue->AsObject();

	if (!MeshOperation.IsValid())
	{
		UE_LOG(LogInstaLOD, Warning, TEXT("InstaLOD: JSON parsing failed."));
		return;
	}

	if (!MeshOperation->HasField(TEXT("Entries")))
	{
		UE_LOG(LogInstaLOD, Warning, TEXT("JSON profile is missing the required 'Entries' field."));
		return;
	}

	const TArray<TSharedPtr<FJsonValue>>* Entries = nullptr;

	if (!MeshOperation->TryGetArrayField(TEXT("Entries"), Entries) ||
		Entries == nullptr ||
		Entries->Num() == 0 ||
		Entries->Num() > 1)
	{
		UE_LOG(LogInstaLOD, Warning, TEXT("InstaLOD: Only JSON profiles with a single mesh operation are supported."));
		return;
	}

	TSharedPtr<FJsonObject> MeshOperationEntry = (*Entries)[0]->AsObject();

	if (!MeshOperationEntry.IsValid())
	{
		UE_LOG(LogInstaLOD, Warning, TEXT("InstaLOD: JSON parsing failed."));
		return;
	}

	if (!MeshOperationEntry->HasField(TEXT("Type")))
	{
		UE_LOG(LogInstaLOD, Warning, TEXT("JSON profile is missing the required 'Type' field."));
		return;
	}

	const FString MeshOperationType = MeshOperationEntry->GetStringField(TEXT("Type"));

	FInstaLODUIModule& InstaLODUIModule = FModuleManager::LoadModuleChecked<FInstaLODUIModule>("InstaLODUI");
	TArray<UInstaLODBaseTool*> Tools = InstaLODUIModule.GetAllTools();

	bool bIsSuccessful = false;
	for (UInstaLODBaseTool *const Tool : Tools)
	{
		if (Tool == nullptr)
			continue;

		bIsSuccessful = Tool->ReadSettingsFromJSONObject(MeshOperationEntry);
		
		if (!bIsSuccessful)
			continue;
		
		InstaLODWindow->SetCurrentActiveTabIndex(Tool->GetOrderId() - 1);
		break;
	}

	FText Title;
	FText MessageText;
	if (bIsSuccessful)
	{
		Title = NSLOCTEXT(LOCTEXT_NAMESPACE, "ProfileLoaded_Title", "Profile Loaded.");
		MessageText = FText::Format(
			NSLOCTEXT(LOCTEXT_NAMESPACE, "ProfileLoaded_Message", "The settings have been successfully loaded from the profile:\n\n{0}"),
			FText::FromString(FilePath)
		);
	}
	else 
	{
		Title = NSLOCTEXT(LOCTEXT_NAMESPACE, "ProfileLoaded_Title", "Profile Not Loaded");
		MessageText = NSLOCTEXT(LOCTEXT_NAMESPACE, "ProfileLoaded_Message", "Failed to load the profile settings.");
		UE_LOG(LogInstaLOD, Error, TEXT("ERROR: InstaLOD for Unreal Engine does not support the profile's Mesh Operation Type, or the profile loaded was not correctly configured/generated. Please make sure that the profile is valid and does not contain any malformed setting"));
	}

	FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, MessageText, Title);
}

void UInstaLODSettings::OpenGettingStarted()
{
	const FText Title = NSLOCTEXT(LOCTEXT_NAMESPACE, "OnlineHelp_Title", "Leaving UE");
	const EAppReturnType::Type Result = FInstaLODModule::MessageDialogOpen(EAppMsgType::YesNo, NSLOCTEXT(LOCTEXT_NAMESPACE, "OnlineHelp_Message", "This will open your default browser. Are you sure?"), Title);

	if (Result == EAppReturnType::No)
		return;

	const FString TheURL = "https://docs.InstaLOD.io/en/Products/Integrations/InstaLOD_For_UE";
	FPlatformProcess::LaunchURL(*TheURL, nullptr, nullptr);
}

void UInstaLODSettings::OpenSupport()
{
	if (!bAuthorized)
		return;

	const FText Title = NSLOCTEXT(LOCTEXT_NAMESPACE, "OnlineHelp_Title", "Leaving UE");
	const EAppReturnType::Type Result = FInstaLODModule::MessageDialogOpen(EAppMsgType::YesNo, NSLOCTEXT(LOCTEXT_NAMESPACE, "OnlineHelp_Message", "This will open your default browser. Are you sure?"), Title);

	if (Result == EAppReturnType::No)
		return;

	const FString LicenseString = LicenseInformation.ToString();

	if (!LicenseString.Contains("[InstaLOD_Support]"))
	{
		FPlatformProcess::LaunchURL(TEXT("https://community.TheAbstract.co/"), nullptr, nullptr);
		return;
	}

	FPlatformProcess::LaunchURL(TEXT("https://cloud2.InstaLOD.io/client/technical-support"), nullptr, nullptr);
}

/************************************************************************/
/* UInstaLODBaseTool Interface											*/
/************************************************************************/

FText UInstaLODSettings::GetFriendlyName() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "SettingsFriendlyName", "?");
}

FText UInstaLODSettings::GetToolBarToolTip() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "SettingsToolTip", "Shows your License Information and more.");
}

FText UInstaLODSettings::GetComboBoxItemName() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "SettingsToolComboBoxItemName", "Setup");
}

FText UInstaLODSettings::GetOperationInformation() const
{
	return NSLOCTEXT(LOCTEXT_NAMESPACE, "SettingsToolOperationInformation", "");
}

int32 UInstaLODSettings::GetOrderId() const
{
	// just a big number so this is always the last
	return 9999;
}

void UInstaLODSettings::FetchLicenseInformation()
{
	FInstaLODModule& InstaLODModule = FModuleManager::LoadModuleChecked<FInstaLODModule>("InstaLODMeshReduction");

	// fetch license information and version
	if (InstaLOD::IInstaLOD* InstaLODAPI = InstaLODModule.GetInstaLODAPI())
	{
		// To get the recent authorization information, IsHostAuthorized function should be called before calling GetAuthorizationInformation.
		// Please read IsHostAuthorized documentation for more information.
		bAuthorized = InstaLODAPI->IsHostAuthorized();
		LicenseInformation = FText::FromString(UTF8_TO_TCHAR(InstaLODAPI->GetAuthorizationInformation()));
		SDKVersion = FText::FromString(UTF8_TO_TCHAR(InstaLODAPI->GetBuildDate()));
	}
}


#undef LOCTEXT_NAMESPACE
