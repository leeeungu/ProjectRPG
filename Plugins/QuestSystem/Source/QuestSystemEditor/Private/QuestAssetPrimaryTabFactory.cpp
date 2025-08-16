#include "QuestAssetPrimaryTabFactory.h"
#include "QuestAssetEditorApp.h"
#include "QuestAsset.h"
#include "IDetailsView.h"
#include "GraphEditor.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"


QuestAssetPrimaryTabFactory::QuestAssetPrimaryTabFactory(TSharedPtr<QuestAssetEditorApp> app) : FWorkflowTabFactory(FName("QuestAssetPrimaryTab"), app)
{
	_app = app;
	TabLabel = FText::FromString(TEXT("Primary"));

	ViewMenuDescription = FText::FromString(TEXT("Display QuestPrimary"));
	ViewMenuTooltip = FText::FromString(TEXT("show the primary view"));
}

TSharedRef<SWidget> QuestAssetPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<QuestAssetEditorApp> app = _app.Pin();
	SGraphEditor::FGraphEditorEvents graphEvents{};
	graphEvents.OnSelectionChanged.BindRaw(app.Get(), &QuestAssetEditorApp::OnGraphSelectedChanged);

	TSharedPtr<SGraphEditor> graphEditor =
		SNew(SGraphEditor)
		.IsEditable(true)
		.GraphEvents(graphEvents)
		.GraphToEdit(app->GetWorkingGraph());
	app->SetWorkingGraphUI(graphEditor);

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			graphEditor.ToSharedRef()
		];
}

FText QuestAssetPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("a primary view for doing primary things"));
}
