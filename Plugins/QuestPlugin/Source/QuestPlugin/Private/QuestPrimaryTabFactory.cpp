#include "QuestPrimaryTabFactory.h"
#include "QuestEditorApp.h"
#include "QuestAsset.h"
#include "IDetailsView.h"

QuestPrimaryTabFactory::QuestPrimaryTabFactory(TSharedPtr<class QuestEditorApp> app) : FWorkflowTabFactory( FName("QuestPrimaryTab"), app)
{
	_app = app;
	TabLabel = FText::FromString(TEXT("Primary"));

	ViewMenuDescription = FText::FromString(TEXT("Display QuestPrimar"));
	ViewMenuTooltip = FText::FromString(TEXT("show the primary niew"));
}

TSharedRef<SWidget> QuestPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<QuestEditorApp> app = _app.Pin();
	FPropertyEditorModule& propertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	FDetailsViewArgs detail{};
	{
		detail.bAllowSearch = false;
		detail.bHideSelectionTip = true;
		detail.bLockable = false;
		detail.bSearchInitialKeyFocus = true;
		detail.bUpdatesFromSelection = false;
		detail.NotifyHook = nullptr;
		detail.bShowOptions = true;
		detail.bShowModifiedPropertiesOption = false;
		detail.bShowScrollBar = false;
	}
	TSharedPtr<IDetailsView> detailview = propertyEditorModule.CreateDetailView(detail);
	detailview->SetObject(app->GetWorkingAsset());

	return SNew(SVerticalBox) + SVerticalBox::Slot().FillHeight(1.0f).HAlign(HAlign_Fill)[detailview.ToSharedRef()];

}

FText QuestPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("a primary view for doing primary things"));
}
