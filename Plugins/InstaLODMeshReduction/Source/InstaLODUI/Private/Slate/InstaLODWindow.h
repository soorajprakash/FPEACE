/**
 * InstaLODWindow.h (InstaLOD)
 *
 * Copyright 2016-2024 InstaLOD GmbH - All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * This file and all it's contents are proprietary and confidential.
 *
 * @file InstaLODWindow.h
 * @copyright 2016-2024 InstaLOD GmbH. All rights reserved.
 * @section License
 */

#ifndef InstaLOD_InstaLODWindow_h
#define InstaLOD_InstaLODWindow_h

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "InstaLODTypes.h"

DECLARE_MULTICAST_DELEGATE(FOnNewSelection)

/**
 * This is the window that is placed inside of a dockable tab.
 * All information and tools will be displayed inside of this window using a DetailView and Customizations.
 */
class SInstaLODWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInstaLODWindow) {}
		SLATE_ARGUMENT(TArray<class UInstaLODBaseTool*>, ToolsToRegister)
	SLATE_END_ARGS()

	SInstaLODWindow();
	~SInstaLODWindow();

	/**
	 * Constructs the UI.
	 *
	 * @param InArgs The arguments.
	 */
	void Construct(const FArguments& InArgs);

	/**
	 * Updates the toolbar.
	 */
	void UpdateToolbar();

	/**
	 * Updates the details view.
	 */
	void UpdateDetailsView();

	/**
	 * Enforces an UI refresh on the details view.
	 */
	void ForceRefreshDetailsView();

	/**
	 * Gets the selected and enabled camera components.
	 *
	 * @return The camera components.
	 */
	TArray<TSharedPtr<FInstaLODMeshComponent>> GetEnabledSelectedCameraComponents() const;

	/**
	 * Gets the selected and enabled mesh components.
	 *
	 * @return The mesh components.
	 */
	TArray<TSharedPtr<FInstaLODMeshComponent>> GetEnabledSelectedMeshComponents() const;

	/**
	 * Gets the selected and enabled components.
	 *
	 * @return The components.
	 */
	TArray<TSharedPtr<FInstaLODMeshComponent>> GetEnabledSelectedComponents() const;

	/**
	 * Gets the selected and enabled static mesh components.
	 *
	 * @return The selected static mesh actors.
	 */
	TArray<AActor*> GetSelectedStaticMeshActors() const;

	/**
	 * Returns the Delegate which is called when the Selection changed.
	 *
	 * @return The delegate.
	 */
	FOnNewSelection& GetOnNewSelectionDelegate() { return OnNewSelectionDelegate; }

	/**
	 * Returns the camera component count.
	 *
	 * @return The camera component count.
	 */
	int32 GetNumCameraComponents() const;

	/**
	 * Returns the static mesh component count.
	 *
	 * @return The static mesh component count.
	 */
	int32 GetNumStaticMeshsComponents() const;

	/**
	 * Returns the skeletal mesh component count.
	 *
	 * @return The skeletal mesh component count.
	 */
	int32 GetNumSkeletalMeshComponents() const;	/**< returns the number of selected skeletal meshs. */

	/**
	 * Sets the current active tab by index.
	 *
	 * @param TabIndex the active tab index.
	 */
	void SetCurrentActiveTabIndex(const int32 TabIndex);

	/**
	 * Called on expand selection.
	 *
	 * @param ExpandRadius the expansion radius.
	 * @return unreal engine event reply.
	 */
	FReply OnExpandSelection(const float ExpandRadius);

	/** Updates and redraws the InstaLOD plugin window. */
	void UpdateWindow();

private:
	void OnActorSelectionChanged(const TArray<UObject*>& NewSelection, bool bForceRefresh);
	void OnToolSelectionChanged(const ECheckBoxState NewCheckedState, int32 ToolIndex);

	/** delegate for the creation of the list view item's widget. */
	TSharedRef<ITableRow> MakeComponentListItemWidget(TSharedPtr<FInstaLODMeshComponent> MeshComponent, const TSharedRef<STableViewBase>& OwnerTable);

	/**
	 * Updates the selected components.
	 */
	void UpdateSelectedComponents();

	ECheckBoxState OnIsToolSelected(const int32 ToolIndex) const;

	void PersistCheckBoxStates();

	void OnLevelSelectionChanged(UObject* Object);

	void OnMapChange(uint32 MapFlags);
	void OnNewCurrentLevel();

	void Reset();

	/** Redraws the plugin window to show the unauthorized view. */
	void DrawUnauthorizedWindow();

	/*
	 * Builds the plugin footer.
	 * 
	 * @return The plugin footer.
	 */
	TSharedRef<SWidget> BuildFooter();

	TSharedPtr<class IDetailsView> DetailView;										/**< Detail Viewer that shows the details of the used tools (UObject). */
	TSharedPtr<SListView<TSharedPtr<FInstaLODMeshComponent>>> SelectedActorList;	/**< The list view for the selected actors. */
	TSharedPtr<SVerticalBox> WindowMainLayout;									/**< The main layout of the window. */
	TArray<TSharedPtr<FInstaLODMeshComponent>> SelectedComponents;					/**< Selected components (cameras, meshes). */
	TArray<class UInstaLODBaseTool*> RegisteredTools;								/**< All registered tools. */
	int32 CurrentTool;																/**< The index of the currently active tool. */
	TArray<UObject*> SelectedObjects;												/**< The list of currently selected objects. */
	TMap<UObject*, ECheckBoxState> PersistedCheckBoxStates;							/**< A map to save the state on the checkBoxes. */
	FOnNewSelection OnNewSelectionDelegate;											/**< Called when the viewport selection changes. */
};

#endif