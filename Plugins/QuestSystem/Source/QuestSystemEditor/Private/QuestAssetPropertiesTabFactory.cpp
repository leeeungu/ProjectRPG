#include "QuestAssetPropertiesTabFactory.h"
#include "QuestAssetEditorApp.h"
#include "QuestAsset.h"
#include "IDetailsView.h"

QuestAssetPropertiesTabFactory::QuestAssetPropertiesTabFactory(TSharedPtr<QuestAssetEditorApp> app) : FWorkflowTabFactory(FName("QuestAssetPropertiesTab"), app)
{
	_app = app;
	TabLabel = FText::FromString(TEXT("Properties"));

	ViewMenuDescription = FText::FromString(TEXT("Display the Properties view for the current asset"));
	ViewMenuTooltip = FText::FromString(TEXT("show the Properties niew"));
}

TSharedRef<SWidget> QuestAssetPropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<QuestAssetEditorApp> app = _app.Pin();
    FDetailsViewArgs detailsViewArgs;
    {
        detailsViewArgs.bAllowSearch = false;
        detailsViewArgs.bHideSelectionTip = true;
        detailsViewArgs.bLockable = false;
        detailsViewArgs.bSearchInitialKeyFocus = true;
        detailsViewArgs.bUpdatesFromSelection = false;
        detailsViewArgs.NotifyHook = nullptr;
        detailsViewArgs.bShowOptions = true;
        detailsViewArgs.bShowModifiedPropertiesOption = false;
        detailsViewArgs.bShowScrollBar = false;
    }

    FPropertyEditorModule& propertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

    TSharedPtr<IDetailsView> detailsView = propertyEditorModule.CreateDetailView(detailsViewArgs);
    detailsView->SetObject(app->GetWorkingAsset());

    return SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        .HAlign(HAlign_Fill)
        [
            detailsView.ToSharedRef()
        ];
  
}

FText QuestAssetPropertiesTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("a Properties view for the current asset"));
}
