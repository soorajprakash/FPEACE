/**
 * InstaLODWindow.cpp (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODWindow.cpp
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */

#include "InstaLODWindow.h"
#include "InstaLODUIPCH.h"

#include "InstaLODModule.h"
#include "Slate/InstaLODPluginStyle.h"
#include "Tools/InstaLODBaseTool.h"
#include "Tools/InstaLODSettings.h"
#include "Tools/InstaLODRemeshTool.h"
#include "Tools/InstaLODImposterizeTool.h"
#include "Tools/InstaLODMaterialMergeTool.h"
#include "Editor/EditorStyle/Private/SlateEditorStyle.h"

#include "Algo/ForEach.h"
#include "LevelEditor.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "IDocumentation.h"

#define LOCTEXT_NAMESPACE "InstaLOD"

SInstaLODWindow::SInstaLODWindow() :
	SCompoundWidget(),
	CurrentTool(0)
{
}

SInstaLODWindow::~SInstaLODWindow()
{
	FLevelEditorModule& LevelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditor.OnActorSelectionChanged().RemoveAll(this);

	// remove all delegates
	USelection::SelectionChangedEvent.RemoveAll(this);
	USelection::SelectObjectEvent.RemoveAll(this);
	FEditorDelegates::MapChange.RemoveAll(this);
	FEditorDelegates::NewCurrentLevel.RemoveAll(this);
}

void SInstaLODWindow::Construct(const FArguments& InArgs)
{
	// reset arrays
	SelectedObjects.Empty();
	SelectedComponents.Empty();

	// bind editor delegate to get updates on selected Actors
	FLevelEditorModule& LevelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditor.OnActorSelectionChanged().AddRaw(this, &SInstaLODWindow::OnActorSelectionChanged);

	// get all currently selected Actors
	USelection* const SelectedActors = GEditor->GetSelectedActors();

	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		if (UObject* const Actor = Cast<UObject>(*Iter))
		{
			SelectedObjects.Add(Actor);
		}
	}

	// reset the selected actor view
	Reset();

	// tools passed by the Module
	RegisteredTools = InArgs._ToolsToRegister;

	// pass each tool a reference to this Window
	for (UInstaLODBaseTool* const RegisteredTool : RegisteredTools)
	{
		if (RegisteredTool == nullptr)
			continue;

		RegisteredTool->SetInstaLODWindow(this);
	}

	// we need the PropertyModule to create a DetailView
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowFavoriteSystem = true;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bShowScrollBar = false;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.bShowOptions = true;


	DetailView = PropertyModule.CreateDetailView(DetailsViewArgs);
	ChildSlot[
		SAssignNew(WindowMainLayout, SVerticalBox)
	];

	UpdateWindow();
}

void SInstaLODWindow::UpdateWindow()
{
	check(WindowMainLayout);

	FInstaLODModule& InstaLODModule = FModuleManager::LoadModuleChecked<FInstaLODModule>("InstaLODMeshReduction");
	const bool bIsAuthorized = InstaLODModule.GetInstaLODAPI() != nullptr ? InstaLODModule.GetInstaLODAPI()->IsHostAuthorized() : false;
	if (!bIsAuthorized)
	{
		DrawUnauthorizedWindow();
		return;
	}

	/// The fnProcessInfromationVisibility lambda function hides the process information when the settings tab is selected.
	const auto fnProcessInfromationVisibility = [this]()->EVisibility
	{
		const UInstaLODBaseTool* const Tool = RegisteredTools[CurrentTool];
		check(Tool);
		return Tool->GetClass() == UInstaLODSettings::StaticClass() ? EVisibility::Collapsed : EVisibility::Visible;
	};

	const FSlateBrush* DropDownIcon = FInstaLODPluginStyle::Get().GetBrush("InstaLODUI.MainComboBoxDownArrowIcon");
	check(DropDownIcon);

	WindowMainLayout->ClearChildren();
	WindowMainLayout->AddSlot()
	.Padding(0.0f)
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(2.5f, 5.0f, 0.0f, 5.0f)
		.AutoHeight()
		.MaxHeight(50)
		[
			SNew(SBox)
			.HAlign(EHorizontalAlignment::HAlign_Left)
			.MaxDesiredWidth(500)
			.MinDesiredHeight(70)
			.MaxDesiredHeight(70)
			.WidthOverride(500)
			.HeightOverride(70)
			[
				SNew(SComboButton)
				.ButtonStyle(FInstaLODPluginStyle::Get(), "InstaLODUI.MainComboboxButton")
				.HasDownArrow(false)
				.ButtonContent()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(10.0f, 0.0f, 5.0f, 0.0f)
					.HAlign(EHorizontalAlignment::HAlign_Left)
					.VAlign(EVerticalAlignment::VAlign_Center)
					[
						SNew(STextBlock)
						.Margin(0.5f)
						.TextStyle(FInstaLODPluginStyle::Get(), "InstaLODUI.MainComboboxButton.DefaultTextStyle")
						.Text_Lambda([this]() {
								const UInstaLODBaseTool* const Tool = RegisteredTools[CurrentTool];

								if (Tool != nullptr)
									return Tool->GetComboBoxItemName();

								return FText::GetEmpty(); 
							})
						.Justification(ETextJustify::Center)
					]
					+ SHorizontalBox::Slot()
					.Padding(5.0f, 0.0f, 10.0f, 0.0f)
					.HAlign(EHorizontalAlignment::HAlign_Right)
					.VAlign(EVerticalAlignment::VAlign_Center)
					.AutoWidth()
					.MaxWidth(25.0f)
					[
						SNew(SImage)
						.DesiredSizeOverride(FVector2D(16.0f, 25.0f))
						.Image(DropDownIcon)
					]
				]
				.OnGetMenuContent_Lambda([this]()
				{
					FMenuBuilder MenuBuilder(true, nullptr);
					MenuBuilder.BeginSection("Operations");

					for (int32 ToolIndex = 0; ToolIndex < RegisteredTools.Num(); ToolIndex++)
					{
						const UInstaLODBaseTool* const Tool = RegisteredTools[ToolIndex];
						if (Tool != nullptr)
						{
							FUIAction ItemAction(FExecuteAction::CreateSP(this, &SInstaLODWindow::SetCurrentActiveTabIndex, ToolIndex));
							MenuBuilder.AddMenuEntry(Tool->GetComboBoxItemName(), TAttribute<FText>(), FSlateIcon(), ItemAction);
						}
					}

					MenuBuilder.EndSection();

					return MenuBuilder.MakeWidget();
				})
			]
		]
		+ SVerticalBox::Slot()
		.FillHeight(5.0f)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				// put a Scrollbox around the content area, in case someone resizes the window smaller
				SNew(SScrollBox)
				+ SScrollBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.MaxHeight(100)
					[
						SNew(SBox)
						.Padding(10.0f, 2.5f, 0.0f, 2.5f)
						.Visibility_Lambda(fnProcessInfromationVisibility)
						.HAlign(HAlign_Fill)
						[
							SNew(STextBlock)
							.Justification(ETextJustify::Left)
							.TextStyle(FInstaLODPluginStyle::Get(), "InstaLODUI.OperationInformation")
							.AutoWrapText(true)
							.Text_Lambda([this]() {

									const UInstaLODBaseTool* const Tool = RegisteredTools[CurrentTool];

									if (Tool != nullptr)
										return Tool->GetOperationInformation();

									return FText::GetEmpty();
								})
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SVerticalBox)
						.Visibility_Lambda(fnProcessInfromationVisibility)
						+SVerticalBox::Slot()
						.AutoHeight()
						.Padding(2.0f, 4.0f, 2.0f, 4.0f)
						[
							SNew(SSeparator)
						]
					]

					+ SVerticalBox::Slot()
					.Padding(10.0f, 2.5f, 0.0f, 0.0f)
					.AutoHeight()
					.MaxHeight(128.0f)
					[
						// list of selected actors
						SAssignNew(SelectedActorList, SListView<TSharedPtr<FInstaLODMeshComponent>>)
						.ListItemsSource(&SelectedComponents)
						.OnGenerateRow(this, &SInstaLODWindow::MakeComponentListItemWidget)
					]
				]
				+ SScrollBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[
					// detail view that shows the details of the used tools (UObject)
					DetailView->AsShared()
				]
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.MaxHeight(30)
		[
			SNew(SBox)
			.MinDesiredHeight(30)
			.HeightOverride(30)
			.MaxDesiredHeight(50)
			[
				SNew(SButton)
				.VAlign(EVerticalAlignment::VAlign_Center)
				.ButtonStyle(FInstaLODPluginStyle::Get(), "InstaLODUI.ButtonPrimary")
				.ButtonColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 47, 141)))
				.OnClicked_Lambda([this]() -> FReply {
						UInstaLODBaseTool* const Tool = RegisteredTools[CurrentTool];
						if (Tool != nullptr)
						{
							Tool->ExecuteMeshOperation();
						}
						return FReply::Handled();
					})
				.IsEnabled_Lambda([this]() -> bool {
						const UInstaLODBaseTool* const Tool = RegisteredTools[CurrentTool];

						if ((Tool != nullptr) && (Tool->GetInstaLODWindow() != nullptr) && (Tool->GetClass() != UInstaLODSettings::StaticClass()))
							return Tool->GetInstaLODWindow()->GetEnabledSelectedMeshComponents().Num() > 0;

						return false;
					})
				[
					SNew(STextBlock)
					.Text(FText::FromString("Execute"))
					.TextStyle(FInstaLODPluginStyle::Get(), "InstaLODUI.ButtonPrimary.BoldTextStyle")
					.Justification(ETextJustify::Center)
				]
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(FMargin(10.0f, 25.0f, 10.0f, 25.0f))
		[
			BuildFooter()
		]
	];

	UpdateToolbar();
	ForceRefreshDetailsView();
}

TSharedRef<SWidget> SInstaLODWindow::BuildFooter()
{
	return SNew(STextBlock)
		.Text(NSLOCTEXT(LOCTEXT_NAMESPACE, "WindowFooter",
			"InstaLOD GmbH 2016 - 2024\n"
			"http://www.InstaLOD.com\n"
			"SDK2024"))
		.Justification(ETextJustify::Center);
}

void SInstaLODWindow::DrawUnauthorizedWindow()
{
	check(WindowMainLayout);
	
	FInstaLODModule& InstaLODModule = FModuleManager::LoadModuleChecked<FInstaLODModule>("InstaLODMeshReduction");
	const bool bIsFloatingLicenseAvailable = InstaLODModule.bIsInstaLODFloatingLicenseAvailable;
	WindowMainLayout->ClearChildren();

	if (!bIsFloatingLicenseAvailable && !InstaLODModule.bAuthorizeWithAnotherAccount)
	{
		const FText TryAgainButtonText = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODFloatingLicenseUnavailableTryAgainButtonText", "Try again");
		const FText AuthorizeInstaLODWithAnotherAccountButtonText = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODFloatingLicenseUnavailableAuthorizeWithAnotherAccount", "Authorize with another account");
		const FText MessageContent = NSLOCTEXT(LOCTEXT_NAMESPACE, "InstaLODFloatingLicenseUnavailable", "You have authorized InstaLOD with a floating license, but the license could not be verified by the Abstract servers.\n\nTo continue using InstaLOD, please make sure that you're connected to the internet and that your license is not in use on another computer.");
		WindowMainLayout->AddSlot()
		.Padding(5.0f)
		.AutoHeight()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.Padding(0.0f, 10.0f)
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(MessageContent)
				.Justification(ETextJustify::Center)
				.TextStyle(FInstaLODPluginStyle::Get(), "InstaLODUI.UnauthorizedWindow.FloatingLicense.UserMessageTextStyle")
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.Padding(0.0f, 10.0f)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Fill)
				[
					SNew(SButton)
					.OnClicked_Lambda([this]()-> FReply{
						// callback to the InstaLODAPI
						FInstaLODModule& InstaLODModule = FModuleManager::LoadModuleChecked<FInstaLODModule>("InstaLODMeshReduction");
						const char* ErrorMessage;
						if (InstaLODModule.ForceLicenseRefreshDelegate(&ErrorMessage))
						{
							InstaLODModule.bIsInstaLODFloatingLicenseAvailable = true;
							InstaLODModule.ForceLicenseRefreshDelegate = nullptr;
							UInstaLODSettings* const SettingsTool = Cast<UInstaLODSettings>(RegisteredTools.Last(/*IndexFromTheEnd:*/0));
							SettingsTool->FetchLicenseInformation();
							UpdateWindow();
							return FReply::Handled();
						}
						const FText Title = NSLOCTEXT(LOCTEXT_NAMESPACE, "AuthorizeError_Title", "Couldn't retrieve InstaLOD Module.");
						const FText Message = FText::FromString(FString(ErrorMessage));
						FInstaLODModule::MessageDialogOpen(EAppMsgType::Ok, FText::Format(NSLOCTEXT(LOCTEXT_NAMESPACE, "AuthorizeError_Message", "{0}"), Message), Title);
						return FReply::Handled();
					})
					.Content()
					[
						SNew(STextBlock)
						.Text(TryAgainButtonText)
						.TextStyle(FInstaLODPluginStyle::Get(), "InstaLODUI.ButtonPrimary.BoldTextStyle")
						.Justification(ETextJustify::Center)
					]
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Fill)
				[
					SNew(SButton)
					.OnClicked_Lambda([this]()-> FReply{
						FInstaLODModule& InstaLODModule = FModuleManager::LoadModuleChecked<FInstaLODModule>("InstaLODMeshReduction");
						InstaLODModule.bAuthorizeWithAnotherAccount = true;
						UpdateWindow();
						return FReply::Handled();
					})
					.Content()
					[
						SNew(STextBlock)
						.Text(AuthorizeInstaLODWithAnotherAccountButtonText)
						.TextStyle(FInstaLODPluginStyle::Get(), "InstaLODUI.ButtonPrimary.BoldTextStyle")
						.Justification(ETextJustify::Center)
					]
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(10.0f, 25.0f, 10.0f, 25.0f))
			[
				BuildFooter()
			]
		];

		return;
	}

	const float kInputTextMinimumWidth = 50.0f;

	const FText WarningText = NSLOCTEXT(LOCTEXT_NAMESPACE, "SettingsWarning",
		"InstaLOD requires a valid license. Please enter your licensing information in the fields below.\n"
		"In order to acquire a license an active internet connection is required.\n"
		"InstaLOD periodically connects to the InstaLOD servers to validate and refresh the license.");

	const FText UserNameLabel = NSLOCTEXT(LOCTEXT_NAMESPACE, "UserNameLabel", "Username");
	const FText PasswordLabel = NSLOCTEXT(LOCTEXT_NAMESPACE, "PasswordLabel", "Password");
	const FText ButtonCaption = NSLOCTEXT(LOCTEXT_NAMESPACE, "AuthorizeWorkStationLabel", "Authorize Workstation");
	const float kPadding = 5.0f;
	WindowMainLayout->AddSlot()
	.Padding(0.0f)
	.AutoHeight()
	[
		SNew(SBorder)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(kPadding, kPadding, kPadding, 20.0f)
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.Padding(kPadding)
				[
					// Info text
					SNew(STextBlock)
					.Text(WarningText)
					.AutoWrapText(true)
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FSlateColor(FLinearColor(FColor::Red)))
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(kPadding)
			[
				// AccountName TextBox
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				[
					SNew(STextBlock)
					.Text(UserNameLabel)
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SEditableTextBox)
					.OnTextChanged_Lambda([this](const FText NewText){
						UInstaLODSettings* const SettingsTool = Cast<UInstaLODSettings>(RegisteredTools.Last(/*IndexFromTheEnd:*/0));
						SettingsTool->AccountName = NewText;
						})
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(kPadding)
			[
				// Password TextBox
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				[
					SNew(STextBlock)
					.Text(PasswordLabel)
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SEditableTextBox)
					.IsPassword(true)
					.OnTextChanged_Lambda([this](const FText NewText){
						UInstaLODSettings* const SettingsTool = Cast<UInstaLODSettings>(RegisteredTools.Last(/*IndexFromTheEnd:*/0));
						check(SettingsTool);
						SettingsTool->SerialPassword = NewText;
						})
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SButton)
				.ButtonStyle(FInstaLODPluginStyle::Get(), "InstaLODUI.ButtonPrimary")
				.ButtonColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 47, 141)))
				.OnClicked_Lambda([this]()-> FReply {
					UInstaLODSettings* const SettingsTool = Cast<UInstaLODSettings>(RegisteredTools.Last(/*IndexFromTheEnd:*/0));
					check(SettingsTool);
					SettingsTool->AuthorizeWorkstation();
					return FReply::Handled();
					})
				.Content()
				[
					SNew(STextBlock)
						.Text(ButtonCaption)
						.TextStyle(FInstaLODPluginStyle::Get(), "InstaLODUI.ButtonPrimary.BoldTextStyle")
						.Justification(ETextJustify::Center)
				]
			]
		]
	];
	return;
}

int32 SInstaLODWindow::GetNumCameraComponents() const
{
	int32 Count = 0;
	Algo::ForEach(SelectedComponents, [&Count](const TSharedPtr<FInstaLODMeshComponent>& InstaLODMeshComponent) { Count += InstaLODMeshComponent->bShouldBeIncluded && InstaLODMeshComponent->CameraComponent.IsValid() ? 1 : 0; });
	return Count;
}

int32 SInstaLODWindow::GetNumStaticMeshsComponents() const
{
	int32 Count = 0;
	Algo::ForEach(SelectedComponents, [&Count](const TSharedPtr<FInstaLODMeshComponent>& InstaLODMeshComponent) { Count += InstaLODMeshComponent->bShouldBeIncluded && InstaLODMeshComponent->StaticMeshComponent.IsValid() ? 1 : 0; });
	return Count;
}

int32 SInstaLODWindow::GetNumSkeletalMeshComponents() const
{
	int32 Count = 0;
	Algo::ForEach(SelectedComponents, [&Count](const TSharedPtr<FInstaLODMeshComponent>& InstaLODMeshComponent) { Count += InstaLODMeshComponent->bShouldBeIncluded && InstaLODMeshComponent->SkeletalMeshComponent.IsValid() ? 1 : 0; });
	return Count;
}

TArray<TSharedPtr<FInstaLODMeshComponent>> SInstaLODWindow::GetEnabledSelectedComponents() const
{
	return SelectedComponents;
}

TArray<AActor*> SInstaLODWindow::GetSelectedStaticMeshActors() const
{
	TArray<AActor*> Actors;
	for (UObject* const SelectedObject : SelectedObjects)
	{
		if (!SelectedObject->IsA<AActor>())
			continue;

		AActor* const Actor = Cast<AActor>(SelectedObject);

		if (!Actor->IsA<AStaticMeshActor>())
			continue;

		Actors.AddUnique(Actor);
	}
	return Actors;
}

TArray<TSharedPtr<FInstaLODMeshComponent>> SInstaLODWindow::GetEnabledSelectedCameraComponents() const
{
	TArray<TSharedPtr<FInstaLODMeshComponent>> CameraComponents;

	for (const TSharedPtr<FInstaLODMeshComponent>& CameraComponent : SelectedComponents)
	{
		// ignore unchecked components
		if (!CameraComponent->bShouldBeIncluded ||
			!CameraComponent->CameraComponent.IsValid())
			continue;

		CameraComponents.Add(CameraComponent);
	}

	return CameraComponents;
}

TArray<TSharedPtr<FInstaLODMeshComponent>> SInstaLODWindow::GetEnabledSelectedMeshComponents() const
{
	TArray<TSharedPtr<FInstaLODMeshComponent>> MeshComponents;

	for (const TSharedPtr<FInstaLODMeshComponent>& MeshComponent : SelectedComponents)
	{
		// ignore unchecked components
		if (!MeshComponent->bShouldBeIncluded)
			continue;

		// require either a static mesh or skeletal mesh
		if (MeshComponent->StaticMeshComponent == nullptr &&
			MeshComponent->SkeletalMeshComponent == nullptr)
			continue;

		// ensure mesh data exists on the component
		if (MeshComponent->StaticMeshComponent != nullptr &&
			MeshComponent->StaticMeshComponent->GetStaticMesh() == nullptr)
			continue;

		if (MeshComponent->SkeletalMeshComponent != nullptr &&
			MeshComponent->SkeletalMeshComponent->GetSkeletalMeshAsset() == nullptr)
			continue;

		MeshComponents.Add(MeshComponent);
	}

	return MeshComponents;
}

void SInstaLODWindow::OnActorSelectionChanged(const TArray<UObject*>& NewSelection, bool bForceRefresh)
{
	SelectedObjects = NewSelection;
	Reset();
}

void SInstaLODWindow::UpdateToolbar()
{
	DetailView->SetObject(nullptr);

	const ISlateStyle& StyleSet = FAppStyle::Get();

	// create a SHorizontalBox that we will fill based on the registered tools
	TSharedRef<SHorizontalBox> HorizontalBox = SNew(SHorizontalBox);
	UInstaLODBaseTool* Settings = nullptr;

	FInstaLODModule& InstaLODModule = FModuleManager::LoadModuleChecked<FInstaLODModule>("InstaLODMeshReduction");
	const bool bIsAuthorized = InstaLODModule.GetInstaLODAPI() ? InstaLODModule.GetInstaLODAPI()->IsHostAuthorized() : false;

	// show the settings when not authorized
	if (bIsAuthorized == false)
	{
		CurrentTool = RegisteredTools.Num() - 1;
	}

	// selection change delegates
	USelection::SelectionChangedEvent.AddRaw(this, &SInstaLODWindow::OnLevelSelectionChanged);
	USelection::SelectObjectEvent.AddRaw(this, &SInstaLODWindow::OnLevelSelectionChanged);
	FEditorDelegates::MapChange.AddSP(this, &SInstaLODWindow::OnMapChange);
	FEditorDelegates::NewCurrentLevel.AddSP(this, &SInstaLODWindow::OnNewCurrentLevel);

	// update the details view
	UpdateDetailsView();
}

void SInstaLODWindow::SetCurrentActiveTabIndex(const int32 TabIndex)
{
	if (!RegisteredTools.IsValidIndex(TabIndex))
		return;

	CurrentTool = TabIndex;
	UpdateDetailsView();
}

void SInstaLODWindow::UpdateDetailsView()
{
	if (RegisteredTools.IsValidIndex(CurrentTool))
	{
		// set the viewed object of the DetailsView
		DetailView->SetObject(RegisteredTools[CurrentTool]);
	}
}

void SInstaLODWindow::ForceRefreshDetailsView()
{
	if (RegisteredTools.IsValidIndex(CurrentTool))
	{
		DetailView->ForceRefresh();
	}
}

void SInstaLODWindow::OnToolSelectionChanged(const ECheckBoxState NewCheckedState, int32 ToolIndex)
{
	if (NewCheckedState != ECheckBoxState::Checked)
		return;

	CurrentTool = ToolIndex;
	UpdateDetailsView();
}

TSharedRef<ITableRow> SInstaLODWindow::MakeComponentListItemWidget(TSharedPtr<FInstaLODMeshComponent> MeshComponent, const TSharedRef<STableViewBase>& OwnerTable)
{
	TSharedPtr<SBox> MeshDataBox;

	const FText OwningActorName = MeshComponent->IsValid() ? FText::FromString(MeshComponent->GetComponent()->GetOwner()->GetName()) : FText();
	const FText ComponentName = MeshComponent->IsValid() ? FText::FromString(MeshComponent->GetComponent()->GetName()) : FText();
	FText MeshName = NSLOCTEXT(LOCTEXT_NAMESPACE, "NoMeshAvailable", "No Mesh Available");

	// fetch persisted state
	ECheckBoxState CheckBoxState = ECheckBoxState::Checked;
	const auto PersistedCheckBoxState = MeshComponent->IsValid() ? PersistedCheckBoxStates.Find(MeshComponent->GetComponent()) : nullptr;
	if (PersistedCheckBoxState)
	{
		CheckBoxState = *PersistedCheckBoxState;
	}

	// get mesh names
	if (MeshComponent->IsValid())
	{
		if (MeshComponent->StaticMeshComponent.IsValid() && MeshComponent->StaticMeshComponent->GetStaticMesh() != nullptr)
		{
			MeshName = FText::FromString(MeshComponent->StaticMeshComponent->GetStaticMesh()->GetName());
		}
		else if (MeshComponent->SkeletalMeshComponent.IsValid() && MeshComponent->SkeletalMeshComponent->GetSkeletalMeshAsset() != nullptr)
		{
			MeshName = FText::FromString(MeshComponent->SkeletalMeshComponent->GetSkeletalMeshAsset()->GetName());
		}
		else if (MeshComponent->CameraComponent.IsValid())
		{
			MeshName = NSLOCTEXT(LOCTEXT_NAMESPACE, "Camera", "Camera");
		}
	}

	MeshDataBox = SNew(SBox)
		[
			// disable UI element if the pointer became invalid
			SNew(SHorizontalBox)
				.IsEnabled((MeshComponent->IsValid()))
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SCheckBox)
						.IsChecked(CheckBoxState)
						.ToolTipText(NSLOCTEXT(LOCTEXT_NAMESPACE, "IncludeMeshInOperation", "If selected, the mesh will be included in the mesh operation."))
						.OnCheckStateChanged_Lambda([/*copy:*/ MeshComponent, this](ECheckBoxState NewState)
							{
								if (!MeshComponent->IsValid())
									return;

								MeshComponent->bShouldBeIncluded = NewState == ECheckBoxState::Checked;
								GetOnNewSelectionDelegate().Broadcast();
							})
				]
				+ SHorizontalBox::Slot()
				.Padding(5.0f, 0.0f, 0.0f, 0.0f)
				.AutoWidth()
				[
					SNew(STextBlock)
						.ColorAndOpacity(FSlateColor(FLinearColor(MeshComponent->CameraComponent.IsValid() ? FColor::Orange : FColor::White)))
						.Text(FText::Format(NSLOCTEXT(LOCTEXT_NAMESPACE, "UIMeshFormatString", "{0} - {1} ({2})"), OwningActorName, MeshName, ComponentName))
				]
		];

	return SNew(STableRow<TSharedPtr<FInstaLODMeshComponent>>, OwnerTable)
		[
			MeshDataBox->AsShared()
		];
}

FReply SInstaLODWindow::OnExpandSelection(const float ExpandRadius)
{
	if (FMath::IsNearlyZero(ExpandRadius))
		return FReply::Handled();

	TArray<AActor*> Actors;
	for (UObject* const SelectedObject : SelectedObjects)
	{
		if (!SelectedObject->IsA<AActor>())
			continue;

		AActor* const Actor = Cast<AActor>(SelectedObject);
		Actors.AddUnique(Actor);
	}

	if (Actors.Num() == 0)
		return FReply::Handled();

	FBox MinimumBoundingBox{ Actors[0]->GetComponentsBoundingBox() };
	for (const AActor* const Actor : Actors)
	{
		MinimumBoundingBox += Actor->GetComponentsBoundingBox();
	}

	const float UnrealEngineUnitToMeterConversionFactor = 100.0f;
	MinimumBoundingBox = MinimumBoundingBox.ExpandBy(ExpandRadius * UnrealEngineUnitToMeterConversionFactor);

	TArray<AActor*> SceneActors;
	UGameplayStatics::GetAllActorsOfClass(Actors[0]->GetWorld(), AActor::StaticClass(), SceneActors);

	GEditor->SelectNone(true, false);
	for (AActor* const SceneActor : SceneActors)
	{
		if (SceneActor == nullptr)
			continue;

		if (MinimumBoundingBox.Intersect(SceneActor->GetComponentsBoundingBox()))
		{
			GEditor->SelectActor(SceneActor, true, true);
		}
	}
	return FReply::Handled();
}

void SInstaLODWindow::UpdateSelectedComponents()
{
	// filter by actors
	TArray<AActor*> Actors;
	for (UObject* const SelectedObject : SelectedObjects)
	{
		if (!SelectedObject->IsA<AActor>())
			continue;

		AActor* const Actor = Cast<AActor>(SelectedObject);
		Actors.AddUnique(Actor);
	}

	// gather selected components relevant for us
	SelectedComponents.Empty();
	for (AActor* const Actor : Actors)
	{
		check(Actor != nullptr);

		// NOTE: if the actor is not a camera controller we also must include all child actors.
		const bool bIsCameraActor = Actor->IsA<ACameraActor>();

		// add all child actors
		if (!bIsCameraActor)
		{
			TArray<UChildActorComponent*> ChildActorComponents;
			Actor->GetComponents<UChildActorComponent>(ChildActorComponents);

			for (UChildActorComponent* const ChildComponent : ChildActorComponents)
			{
				if (ChildComponent == nullptr)
					continue;

				if (AActor* const ChildActor = ChildComponent->GetChildActor())
				{
					if (ChildActor->IsA<ACameraActor>())
						continue;

					Actors.AddUnique(ChildActor);
				}
			}
		}

		// allocate instalod components
		{
			TArray<USceneComponent*> SceneComponents;
			Actor->GetComponents<USceneComponent>(SceneComponents);

			for (USceneComponent* const SceneComponent : SceneComponents)
			{
				if (SceneComponent == nullptr)
					continue;

				TSharedPtr<FInstaLODMeshComponent> InstaLODMeshComponent;

				// only include compatible components
				if (SceneComponent->IsA<UStaticMeshComponent>() && !bIsCameraActor)
				{
					InstaLODMeshComponent = TSharedPtr<FInstaLODMeshComponent>(new FInstaLODMeshComponent(Cast<UStaticMeshComponent>(SceneComponent)));
				}
				else if (SceneComponent->IsA<USkeletalMeshComponent>() && !bIsCameraActor)
				{
					InstaLODMeshComponent = TSharedPtr<FInstaLODMeshComponent>(new FInstaLODMeshComponent(Cast<USkeletalMeshComponent>(SceneComponent)));
				}
				else if (SceneComponent->IsA<UCameraComponent>())
				{
					InstaLODMeshComponent = TSharedPtr<FInstaLODMeshComponent>(new FInstaLODMeshComponent(Cast<UCameraComponent>(SceneComponent)));
				}
				else
				{
					continue;
				}

				const auto PersistedCheckBoxState = PersistedCheckBoxStates.Find(InstaLODMeshComponent->GetComponent());
				if (PersistedCheckBoxState)
				{
					InstaLODMeshComponent->bShouldBeIncluded = (*PersistedCheckBoxState == ECheckBoxState::Checked);
				}
				SelectedComponents.Add(InstaLODMeshComponent);
			}
		}
	}

	// rebuild list widget
	if (SelectedActorList.IsValid())
	{
		SelectedActorList->ClearSelection();
		SelectedActorList->RequestListRefresh();
	}

	OnNewSelectionDelegate.Broadcast();
}

ECheckBoxState SInstaLODWindow::OnIsToolSelected(const int32 ToolIndex) const
{
	return CurrentTool == ToolIndex ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SInstaLODWindow::PersistCheckBoxStates()
{
	PersistedCheckBoxStates.Empty();
	for (const TSharedPtr<FInstaLODMeshComponent>& SelectedComponent : SelectedComponents)
	{
		check(SelectedComponent.IsValid());

		const ECheckBoxState CheckBoxState = SelectedComponent->bShouldBeIncluded ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;

		PersistedCheckBoxStates.Add(SelectedComponent->GetComponent(), CheckBoxState);
	}
}

void SInstaLODWindow::OnLevelSelectionChanged(UObject* Object)
{
	Reset();
}

void SInstaLODWindow::OnMapChange(uint32 MapFlags)
{
	Reset();
}

void SInstaLODWindow::OnNewCurrentLevel()
{
	Reset();
}

void SInstaLODWindow::Reset()
{
	PersistCheckBoxStates();
	UpdateSelectedComponents();
}

#undef LOCTEXT_NAMESPACE
