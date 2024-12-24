/**
 * InstaLODModule.cpp (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODModule.cpp
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */

#include "InstaLODMeshReductionPCH.h"
#include "InstaLODModule.h"

#include "Runtime/Core/Public/Features/IModularFeatures.h"
#include "ComponentReregisterContext.h"
#include "Slate/InstaLODPluginStyle.h"
#include "Misc/ConfigUtilities.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Notifications/NotificationManager.h"
#include "EditorSupportDelegates.h"
#include "PersonaModule.h"
#include "StaticMeshEditorModule.h"
#include "Widgets/Input/STextComboBox.h"
#include "LevelEditor.h"

#include "Interfaces/IPluginManager.h"

#include "Editor/EditorStyle/Private/SlateEditorStyle.h"

#define GetInstaLODPtr()(GInstaLOD.Get())

TUniquePtr<FInstaLOD> GInstaLOD;

const FString InstaLODShared::TargetedUnrealEngineVersion = TEXT("Compatible UE Version: 5.4, 5.3, 5.2");
FString InstaLODShared::Version = TEXT("6.2");
FString InstaLODShared::LicenseInformation = TEXT("Unauthorized");
InstaLOD::IInstaLOD::pfnForceLicenseRefreshCallback FInstaLODModule::ForceLicenseRefreshDelegate = nullptr;
bool FInstaLODModule::bIsInstaLODFloatingLicenseAvailable = true;
bool FInstaLODModule::bAuthorizeWithAnotherAccount = false;
bool FInstaLODModule::bAuthorizeInstaLODLater = false;

#define LOCTEXT_NAMESPACE "InstaLOD"
#define UE_INSTALOD_LIBRARY_NAME "InstaLOD"

// library file name depends on target platform
#if defined (INSTALOD_LIB_DYNAMIC)
#	if PLATFORM_WINDOWS
#		if PLATFORM_64BITS
#			define INSTALOD_LIB_DYNAMIC_PATH	TEXT("DLLs/Win64/") TEXT(UE_INSTALOD_LIBRARY_NAME) TEXT(".dll")
#		endif
#   elif PLATFORM_MAC
#       define INSTALOD_LIB_DYNAMIC_PATH		TEXT("DLLs/Mac/lib" UE_INSTALOD_LIBRARY_NAME ".dylib")
#   elif PLATFORM_LINUX
#		define INSTALOD_LIB_DYNAMIC_PATH		TEXT("DLLs/Linux/lib" UE_INSTALOD_LIBRARY_NAME ".so")
#	endif
#	ifndef INSTALOD_LIB_DYNAMIC_PATH
#		error Platform not supported by InstaLOD
#	endif
#else
#	error InstaLOD supports only dynamically linking the SDK
#endif

IMeshReduction* FInstaLODModule::GetSkeletalMeshReductionInterface() { return GetInstaLODPtr(); }
IMeshReduction* FInstaLODModule::GetStaticMeshReductionInterface() { return GetInstaLODPtr(); }
IMeshMerging* FInstaLODModule::GetMeshMergingInterface() { return GetInstaLODPtr(); }
IInstaLOD* FInstaLODModule::GetInstaLODInterface() { return GetInstaLODPtr(); }

void FInstaLODModule::StartupModule()
{
	bIsInstaLODFloatingLicenseAvailable = true;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 2
	{
		const FString InstaLODPluginName = TEXT("InstaLODMeshReduction");
		UE_LOG(LogInstaLOD, Log, TEXT("InstaLOD enforce %s plugin."), *InstaLODPluginName);

		IConsoleManager& ConsoleManager = IConsoleManager::Get();

		// NOTE: Due to a bug in UE 5.2, where the settings are loaded from config, but are overwritten by ConsoleVariable reinitialization, the settings are never correctly applied.
		// In order to still be able to load InstaLOD we enforce the values before the MeshReductionManagerModule is configured.
		IConsoleVariable* const MeshReductionCVar = ConsoleManager.FindConsoleVariable(TEXT("r.MeshReductionModule"));
		MeshReductionCVar->Set(*InstaLODPluginName, EConsoleVariableFlags::ECVF_SetByCode);

		IConsoleVariable* const SkeletalMeshReductionCVar = ConsoleManager.FindConsoleVariable(TEXT("r.SkeletalMeshReductionModule"));
		SkeletalMeshReductionCVar->Set(*InstaLODPluginName, EConsoleVariableFlags::ECVF_SetByCode);

		IConsoleVariable* const ProxyMeshReductionCVar = ConsoleManager.FindConsoleVariable(TEXT("r.ProxyLODMeshReductionModule"));
		ProxyMeshReductionCVar->Set(*InstaLODPluginName, EConsoleVariableFlags::ECVF_SetByCode);
	}
#endif

	InstaLODAPI = nullptr;

#if defined (INSTALOD_LIB_DYNAMIC)
	const FString PluginBaseDir = IPluginManager::Get().FindPlugin("InstaLODMeshReduction")->GetBaseDir();
	const auto LibraryFileName = INSTALOD_LIB_DYNAMIC_PATH;
	bool bIsLoaded = false;
	
	// try loading from relative plugins path first
	FString LibraryPath = FPaths::Combine(*PluginBaseDir, LibraryFileName);
	
	// if relative path is not found, then try to load from engine dir
	if (!FPaths::FileExists(LibraryPath))
	{
		LibraryPath = FPaths::Combine(*FPaths::EnginePluginsDir(), TEXT("InstaLODMeshReduction/"), LibraryFileName);
	}
	
	// if engine path is not found, then try to load from project dir
	if (!FPaths::FileExists(LibraryPath))
	{
		LibraryPath = FPaths::Combine(*FPaths::ProjectPluginsDir(), TEXT("InstaLODMeshReduction/"), LibraryFileName);
	}
	
	if (FPaths::FileExists(LibraryPath))
	{
		if (void* pLibraryHandle = FPlatformProcess::GetDllHandle(*LibraryPath))
		{
			if (pfnGetInstaLOD pGetInstaLOD = (pfnGetInstaLOD)FPlatformProcess::GetDllExport(pLibraryHandle, TEXT("GetInstaLOD")))
			{
				if (pGetInstaLOD(INSTALOD_API_VERSION, &InstaLODAPI) != 0)
				{
					bIsLoaded = true;
					UE_LOG(LogInstaLOD, Log, TEXT("%s"), UTF8_TO_TCHAR(InstaLODAPI->GetBuildDate()));
				}
			}
		}
	}
	
	if (!bIsLoaded || InstaLODAPI == nullptr)
	{
		UE_LOG(LogInstaLOD, Fatal, TEXT("Unable to load InstaLOD Library. Please ensure that the InstaLOD library is in the library directory. If InstaLOD still fails to load please reinstall InstaLOD."));
	}
#else
	GetInstaLOD(INSTALOD_API_VERSION, &InstaLODAPI);
	UE_LOG(LogInstaLOD, Display, TEXT("%s"), UTF8_TO_TCHAR(InstaLODAPI->GetBuildDate()));
#endif
	
	GInstaLOD.Reset(FInstaLOD::Create(InstaLODAPI));
	
#if PLATFORM_WINDOWS
	// NOTE: InstaLOD.dll is built with MB, this is causing the secureCRT to panic when logging to a unicode stdout during cook
	// therefore it is important to disable all stdout logs
	InstaLODAPI->SetStandardOutputEnabled(false);
#endif

	InstaLODAPI->SetLicenseUnavailableCallback(&FInstaLODModule::LicenseUnavailableCallback);
	if (!InstaLODAPI->InitializeAuthorization("InstaLOD", nullptr))
	{
		UE_LOG(LogInstaLOD, Fatal, TEXT("Failed to initialize InstaLOD authorization module with error: %s"), UTF8_TO_TCHAR(InstaLODAPI->GetAuthorizationInformation()));
	}
 
	// setup the toolbar button
	if (GInstaLOD != nullptr)
	{
		InstaLODShared::Version = UTF8_TO_TCHAR(InstaLODAPI->GetBuildDate());
		InstaLODShared::LicenseInformation = UTF8_TO_TCHAR(InstaLODAPI->GetAuthorizationInformation());
		
		UE_LOG(LogInstaLOD, Log, TEXT("%s"), *InstaLODShared::LicenseInformation);
		
		FInstaLODPluginStyle::Initialize();
		FInstaLODPluginStyle::ReloadTextures();
		
		InstallHooks();
		
		IModularFeatures::Get().RegisterModularFeature(IMeshReductionModule::GetModularFeatureName(), this);

		const FString BaseInstaLODMeshReductionIni = FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectPluginsDir() + "InstaLODMeshReduction/Config/BaseInstaLODMeshReduction.ini");
		UE::ConfigUtilities::ApplyCVarSettingsFromIni(TEXT("Startup"), *BaseInstaLODMeshReductionIni, ECVF_SetByConsoleVariablesIni);
	}
}

void FInstaLODModule::LicenseUnavailableCallback(InstaLOD::IInstaLOD::pfnForceLicenseRefreshCallback ForceLicenseRefreshCallback)
{
	FFunctionGraphTask::CreateAndDispatchWhenReady([/*copy:*/ForceLicenseRefreshCallback]()
		{
			FInstaLODModule& InstaLODModule = FModuleManager::LoadModuleChecked<FInstaLODModule>("InstaLODMeshReduction");
			if (InstaLODModule.bAuthorizeInstaLODLater)
				return;

			InstaLODModule.bIsInstaLODFloatingLicenseAvailable = false;
			InstaLODModule.bAuthorizeWithAnotherAccount = false;
			InstaLODModule.ForceLicenseRefreshDelegate = ForceLicenseRefreshCallback;
			InstaLODShared::OpenFloatingLicenseUnavailableWindowModal(ForceLicenseRefreshCallback);
		}, TStatId(), /*InPrerequisites:*/nullptr, ENamedThreads::GameThread);
}

void FInstaLODModule::ShutdownModule()
{
	if (GInstaLOD != nullptr && GInstaLOD->GetInstaLOD() != nullptr)
	{
		GInstaLOD->GetInstaLOD()->Dealloc();
	}
	GInstaLOD = nullptr;
	ForceLicenseRefreshDelegate = nullptr;
	FInstaLODPluginStyle::Shutdown();
	
	IModularFeatures::Get().UnregisterModularFeature(IMeshReductionModule::GetModularFeatureName(), this);
}

void FInstaLODModule::InstallHooks()
{
	LateHooksDelegateHandle = FEditorSupportDelegates::UpdateUI.AddLambda([this]() { this->InstallHooksLate(); });
}

void FInstaLODModule::InstallHooksLate()
{	
	FEditorSupportDelegates::UpdateUI.Remove(LateHooksDelegateHandle);

	if (GInstaLOD->GetInstaLOD()->IsHostAuthorized())
		return;
	
	UE_LOG(LogInstaLOD, Log, TEXT("Machine not authorized: %s"), UTF8_TO_TCHAR(GInstaLOD->GetInstaLOD()->GetAuthorizationInformation()));
}

EAppReturnType::Type FInstaLODModule::MessageDialogOpen(const EAppMsgType::Type Type, const FText& Message, const FText& Title, const EInstaLODMessageCategory::Type MessageCategory)
{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 2
	return FMessageDialog::Open((EAppMsgCategory)MessageCategory, Type, Message, Title);
#else
	return FMessageDialog::Open(Type, Message, &Title);
#endif
}

class SInstaLODDialogWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInstaLODDialogWidget){}
	SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_END_ARGS()
	
	void Construct( const FArguments& InArgs )
	{
		TSharedPtr<SScrollBox> ScrollBox;
		
		this->ChildSlot
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SAssignNew(ScrollBox, SScrollBox)
			]
		];
		
		ScrollBox->AddSlot()
		[
			InArgs._Content.Widget
		];
	}
	
	/** Sets the window of this dialog. */
	void SetWindow(TSharedPtr<SWindow> InWindow )
	{
		MyWindow = InWindow;
	}
	
	UNREALED_API static void OpenDialog(const FText& InDialogTitle, const TSharedRef< SWidget >& DisplayContent)
	{
		TSharedPtr<SWindow> Window;
		TSharedPtr<SInstaLODDialogWidget> InstaLODDialogWidget;
		
		Window = SNew(SWindow)
		.Title(InDialogTitle)
		.SizingRule(ESizingRule::Autosized)
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		[
			SNew(SBorder)
			.Padding(4.0f)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				SAssignNew(InstaLODDialogWidget, SInstaLODDialogWidget)
				[
					DisplayContent
				]
			]
		];
		
		InstaLODDialogWidget->SetWindow(Window);
		
		FSlateApplication::Get().AddWindow(Window.ToSharedRef());
	}

private:
	/** Pointer to the containing window. */
	TWeakPtr< SWindow > MyWindow;
};

void InstaLODShared::OpenFloatingLicenseUnavailableWindowModal(InstaLOD::IInstaLOD::pfnForceLicenseRefreshCallback ForceLicenseRefreshCallback)
{
	const FText MessageTitle = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODAuthorization", "InstaLOD License Unavailable");
	const FText MessageContent = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODFloatingLicenseUnavailable", "You have authorized InstaLOD with a floating license, but the license could not be verified by the Abstract servers.\n\nTo continue using InstaLOD, please make sure that you're connected to the internet and that your license is not in use on another computer.");
	const FText TryAgainButtonText = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODFloatingLicenseUnavailableTryAgainButtonText", "Try again");
	const FText AuthorizeInstaLODLaterButtonText = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODFloatingLicenseUnavailableAuthorizeInstaLODLater", "Authorize InstaLOD later");

	const static FWindowStyle WindowStyle = FInstaLODPluginStyle::Get().GetWidgetStyle<FWindowStyle>("InstaLODMeshReduction.FloatingLicense.Window.Style");

	TSharedPtr<SWindow> Window = SNew(SWindow)
		.Style(&WindowStyle)
		.CreateTitleBar(false)
		.SizingRule(ESizingRule::Autosized)
		.SupportsMaximize(false)
		.SupportsMinimize(false);

	TWeakPtr<SWindow> WindowWeakPtr = Window.ToWeakPtr();

	Window->SetContent(
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("NoBorder"))
			.Padding(20.0f, 0.0f)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.Padding(0.0f, 35.0f, 0.0f, 40.0f)
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					//NOTE: we use the text instead of logo as we show the window so early that UE does not render images at that stage. We need to decide at which stage we set the LicenseUnavailableCallback so that UE can render the images (InstallHooksLate).
					+SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(MessageTitle)
						.TextStyle(FInstaLODPluginStyle::Get(), "InstaLODMeshReduction.FloatingLicense.Window.Title")
					]
				]
				+ SVerticalBox::Slot()
				.Padding(0.0f, 0.0f, 0.0f, 20.0f)
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(MessageContent)
					.ColorAndOpacity(FLinearColor::White)
				]
				+SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.Padding(0.0f, 0.0f, 0.0f, 20.0f)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SButton)
						.ButtonStyle(FAppStyle::Get(), "NoBorder")
						.Content()
						[
							SNew(SOverlay)
								+ SOverlay::Slot()
								[
									SNew(SImage)
										.Image_Lambda([]() -> const FSlateBrush* {
										return FInstaLODPluginStyle::Get().GetBrush("InstaLODMeshReduction.FloatingLicense.Button.Normal");
											})
								]
								+ SOverlay::Slot()
								.Padding(20.0f, 10.0f)
								[
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.HAlign(HAlign_Center)
										.AutoWidth()
										[
											SNew(STextBlock)
												.Text(TryAgainButtonText)
												.Justification(ETextJustify::Center)
										]
								]
						]
						.OnClicked_Lambda([/*copy:*/ForceLicenseRefreshCallback, /*copy:*/WindowWeakPtr]()
						{
							if (ForceLicenseRefreshCallback == nullptr)
								return FReply::Handled();

							const char* ErrorLog;
							if (ForceLicenseRefreshCallback(&ErrorLog))
							{
								if (const TSharedPtr<SWindow> Window = WindowWeakPtr.Pin())
								{
									Window->RequestDestroyWindow();
								}
								return FReply::Handled();
							}

							const FString ErrorString(ErrorLog);
							const FText ErrorMessageTitle = LOCTEXT("InstaLODReAuthorizationErrorMessages", "InstaLOD: Failed to verify the license");
							FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, FText::FromString(ErrorString), ErrorMessageTitle, EInstaLODMessageCategory::Error);
							return FReply::Handled();
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SButton)
						.ButtonStyle(FAppStyle::Get(), "NoBorder")
						.Content()
						[
							SNew(SOverlay)
								+ SOverlay::Slot()
								[
									SNew(SImage)
										.Image_Lambda([]() -> const FSlateBrush* {
										return FInstaLODPluginStyle::Get().GetBrush("InstaLODMeshReduction.FloatingLicense.Button.Normal");
											})
								]
								+ SOverlay::Slot()
								.Padding(20.0f, 10.0f)
								[
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.HAlign(HAlign_Center)
										.AutoWidth()
										[
											SNew(STextBlock)
												.Text(AuthorizeInstaLODLaterButtonText)
												.Justification(ETextJustify::Center)
										]
								]
						]
						.OnClicked_Lambda([/*copy:*/WindowWeakPtr]()
						{
							if (const TSharedPtr<SWindow> Window = WindowWeakPtr.Pin())
							{
								Window->RequestDestroyWindow();
							}
							static const FName kInstaLODWindowTabName("InstaLODWindow");
							TSharedPtr<SDockTab> LibraryTab = FGlobalTabmanager::Get()->FindExistingLiveTab(kInstaLODWindowTabName);
							if (LibraryTab == nullptr)
								return FReply::Handled();

							LibraryTab->RequestCloseTab();
							return FReply::Handled();
						})
					]
				]
			]
		);

	GEditor->EditorAddModalWindow(Window.ToSharedRef());
}

void InstaLODShared::OpenAuthorizationWindowModal()
{
	TSharedRef<SEditableText> UsernameEditableText = SNew(SEditableText);
	TSharedRef<SEditableText> PasswordEditableText = SNew(SEditableText).IsPassword(true);
	
	const FText MessageTitle = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODAuthorization", "InstaLOD: Manage License");
	const FText MessageContent = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODAuthorizationMessage", "This machine is not authorized for InstaLOD.\nPlease enter your license data to authorize this machine or remove InstaLOD from your Unreal Engine project.");

	TSharedPtr<SWindow> Window = SNew(SWindow)
		.Title(MessageTitle)
		.SizingRule(ESizingRule::Autosized)
		.SupportsMaximize(false)
		.SupportsMinimize(false);

	TWeakPtr<SWindow> WindowWeakPtr = Window.ToWeakPtr();

	/// The fnAuthorizeMachineClickLambda lambda authorizes the workstation and closes the authorization window upon successful authorization.
	const auto fnAuthorizeMachineClickLambda = [/*copy: */ UsernameEditableText, /*copy: */ PasswordEditableText, /*copy: */ WindowWeakPtr]()
	{
		const FString Username = UsernameEditableText->GetText().ToString();
		const FString Password = PasswordEditableText->GetText().ToString();

		if (!GInstaLOD->GetInstaLOD()->AuthorizeMachine(TCHAR_TO_UTF8(*Username), TCHAR_TO_UTF8(*Password)))
		{
			// NOTE: the authorization information contains information about the error
			InstaLODShared::LicenseInformation = UTF8_TO_TCHAR(GInstaLOD->GetInstaLOD()->GetAuthorizationInformation());

			const FText ErrorMessageContents = FText::FromString(InstaLODShared::LicenseInformation);
			const FText ErrorMessageTitle = LOCTEXT("InstaLODAcquiredLicenseFailTitle", "InstaLOD: Authorization Failed");

			FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, ErrorMessageContents, ErrorMessageTitle, EInstaLODMessageCategory::Error);
		}
		else
		{
			InstaLODShared::LicenseInformation = UTF8_TO_TCHAR(GInstaLOD->GetInstaLOD()->GetAuthorizationInformation());
			if (const TSharedPtr<SWindow> Window = WindowWeakPtr.Pin())
			{
				Window->RequestDestroyWindow();
			}
		}
		
		return FReply::Handled();
	};
	
	const TSharedRef<SWidget> DisplayContent =
	SNew(SVerticalBox)
	+SVerticalBox::Slot()
	.AutoHeight()
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Top)
	[
		// InstaLOD logo
		SNew(SImage)
		.Image(FInstaLODPluginStyle::Get().GetBrush("InstaLODMeshReduction.LogoSmall"))
	]
	
	+SVerticalBox::Slot()
	.AutoHeight()
	.Padding(10, 170)
	.VAlign(VAlign_Center)
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.Padding(0.0f, 0.0f, 0.0f, 20.0f)
		.AutoHeight()
		[
			SNew(STextBlock)
			.Justification(ETextJustify::Center)
			.Text(MessageContent)
		]
		+ SVerticalBox::Slot()
		.Padding(0.0f, 0.0f, 0.0f, 12.0f)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.FillWidth(0.2f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("InstaLODAuthorizationUserEmail", "Email or Username"))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(0.8f)
			.VAlign(VAlign_Center)
			[
				SNew(SBorder)
				.Padding(5.0f)
				.BorderImage(FAppStyle::GetBrush("Menu.Background"))
				[
					UsernameEditableText
				]
			]
		]
		+ SVerticalBox::Slot()
		.Padding(0.0f, 0.0f, 0.0f, 0.0f)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(0.2f)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("InstaLODAuthorizationPassword", "Password"))
			]
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.FillWidth(0.8f)
			[
				SNew(SBorder)
				.Padding(5.0f)
				.BorderImage(FAppStyle::GetBrush("Menu.Background"))
				[
					PasswordEditableText
				]
			]
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.Padding(0.0f, 20.0f, 0.0f, 0.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("InstaLODAuthorizationAuthorize", "Authorize"))
			.OnClicked_Lambda(fnAuthorizeMachineClickLambda)
		]
	];

	TSharedRef<SInstaLODDialogWidget> InstaLODDialogWidget =
	SNew(SInstaLODDialogWidget)
	[
		DisplayContent
	];
	
	Window->SetContent(
		SNew(SBorder)
		.Padding(10.0f)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			InstaLODDialogWidget
		]
	);
	InstaLODDialogWidget->SetWindow(Window);
	
	GEditor->EditorAddModalWindow(Window.ToSharedRef());
	
	if (!GInstaLOD->GetInstaLOD()->IsHostAuthorized())
	{
		if (FInstaLODModule::MessageDialogOpen(EAppMsgType::OkCancel, MessageContent, MessageTitle) == EAppReturnType::Ok)
		{
			// we are authorized now!
		}
		else
		{
			// restart the auth loop
			OpenAuthorizationWindowModal();
		}
	}
	else
	{
		const FText AuthorizedMessageContent = FText::Format(LOCTEXT("InstaLODAuthorized", "The workstation has been successfully authorized.\n\n{0}"), FText::FromString(InstaLODShared::LicenseInformation));
		FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, AuthorizedMessageContent, MessageTitle, EInstaLODMessageCategory::Success);
	}
}

void InstaLODShared::OpenDeauthorizationWindowModal()
{
	const FText MessageTitle = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODDeauthorizeNoLicense", "InstaLOD: Deauthorize");
	
	if (!GInstaLOD->GetInstaLOD()->IsHostAuthorized())
	{
		const FText ErrorMessageContents = FText::FromString(InstaLODShared::LicenseInformation);
		FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, ErrorMessageContents, MessageTitle);
		return;
	}
	
	TSharedRef<SEditableText> UsernameEditableText = SNew(SEditableText);
	TSharedRef<SEditableText> PasswordEditableText = SNew(SEditableText).IsPassword(true);
	
	const FText MessageContent = LOCTEXT("InstaLODDeauthorizationMessage", "Please enter your license data to deauthorize this machine.\n");
	const FText MessageFooter = LOCTEXT("InstaLODDeauthorizationFooter", "Visit http://www.InstaLOD.com or contact hello@InstaLOD.com\nfor information on how to obtain a valid InstaLOD license.");
	
	TSharedPtr< SWindow > Window = SNew(SWindow)
		.Title(MessageTitle)
		.SizingRule(ESizingRule::Autosized)
		.SupportsMaximize(false)
		.SupportsMinimize(false);

	TWeakPtr<SWindow> WindowWeakPtr = Window.ToWeakPtr();

	/// The fnDauthorizeMachineClickLambda lambda deauthorizes the workstation and closes the deauthorization window upon successful deauthorization.
	const auto fnDauthorizeMachineClickLambda = [/*copy: */ UsernameEditableText, /*copy: */ PasswordEditableText, /*copy: */ MessageTitle, /*copy: */ WindowWeakPtr]()
	{
		const FString Username = UsernameEditableText->GetText().ToString();
		const FString Password = PasswordEditableText->GetText().ToString();
		
		if (!GInstaLOD->GetInstaLOD()->DeauthorizeMachine(TCHAR_TO_UTF8(*Username), TCHAR_TO_UTF8(*Password)))
		{
			// NOTE: the authorization information contains information about the error
			InstaLODShared::LicenseInformation = UTF8_TO_TCHAR(GInstaLOD->GetInstaLOD()->GetAuthorizationInformation());
			
			const FText ErrorMessageContents = FText::FromString(InstaLODShared::LicenseInformation);
			const FText ErrorMessageTitle = LOCTEXT("InstaLODDeauthorizationFailedTitle", "InstaLOD: Deauthorization failed");
			FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, ErrorMessageContents, ErrorMessageTitle);
		}
		else
		{
			const FText SuccessMessageContents = FText::Format(LOCTEXT("InstaLODDeauthorized", "InstaLOD license removed and machine deauthorized.\n"), FText::FromString(InstaLODShared::LicenseInformation));
			FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, SuccessMessageContents, MessageTitle);
			if (const TSharedPtr<SWindow> Window = WindowWeakPtr.Pin())
			{
				Window->RequestDestroyWindow();
			}
		}
		
		return FReply::Handled();
	};
	
	const TSharedRef<SWidget> DisplayContent =
	SNew(SVerticalBox)
	+SVerticalBox::Slot()
	.AutoHeight()
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	.Padding(10.0f)
	[
		// InstaLOD logo
		SNew(SImage)
		.Image(FInstaLODPluginStyle::Get().GetBrush("InstaLODMeshReduction.LogoSmall"))
	]
	
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(STextBlock)
		.Justification(ETextJustify::Center)
		.Text(MessageContent)
	]
	
	+SVerticalBox::Slot()
	.Padding(0.0f, 20.0f, 0.0f, 0.0f)
	.AutoHeight()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("InstaLODAuthorizationUserEmail", "Email or Username"))
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(SBorder)
		.Padding(5.0f)
		.BorderImage(FAppStyle::GetBrush("Menu.Background"))
		[
			UsernameEditableText
		]
	]
	
	+SVerticalBox::Slot()
	.Padding(0.0f, 10.0f, 0.0f, 0.0f)
	.AutoHeight()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("InstaLODAuthorizationPassword", "Password"))
	]
	+SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(SBorder)
		.Padding(5.0f)
		.BorderImage(FAppStyle::GetBrush("Menu.Background"))
		[
			PasswordEditableText
		]
	]
	
	+SVerticalBox::Slot()
	.HAlign(HAlign_Right)
	.AutoHeight()
	.Padding(0.0f, 10.0f, 0.0f, 0.0f)
	[
		SNew(SButton)
		.Text(LOCTEXT("InstaLODAuthorizationAuthorize", "Deauthorize") )
		.OnClicked_Lambda(fnDauthorizeMachineClickLambda)
	]
	
	+SVerticalBox::Slot()
	.Padding(0.0f, 20.0f, 0.0f, 20.0f)
	.AutoHeight()
	[
		SNew(STextBlock)
		.Text(MessageFooter)
	];
	
	TSharedRef<SInstaLODDialogWidget> InstaLODDialogWidget =
	SNew(SInstaLODDialogWidget)
	[
		DisplayContent
	];
	
	Window->SetContent(
		SNew(SBorder)
		.Padding(10.0f)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			InstaLODDialogWidget
		]
	);
	InstaLODDialogWidget->SetWindow(Window);
	
	GEditor->EditorAddModalWindow(Window.ToSharedRef());
}


IMPLEMENT_MODULE(FInstaLODModule, InstaLODMeshReduction);

#undef LOCTEXT_NAMESPACE
