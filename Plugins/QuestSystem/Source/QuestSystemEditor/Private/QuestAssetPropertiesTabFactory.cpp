#include "QuestAssetPropertiesTabFactory.h"
#include "QuestAssetEditorApp.h"
#include "QuestAsset.h"
#include "IDetailsView.h"
#include "QuestNodeInfo.h"

QuestAssetPropertiesTabFactory::QuestAssetPropertiesTabFactory(TSharedPtr<QuestAssetEditorApp> app) : FWorkflowTabFactory(FName("QuestAssetPropertiesTab"), app)
{
	_app = app;
	TabLabel = FText::FromString(TEXT("Properties"));

	ViewMenuDescription = FText::FromString(TEXT("Display the Properties view for the current asset"));
	ViewMenuTooltip = FText::FromString(TEXT("show the Properties view"));
}

TSharedRef<SWidget> QuestAssetPropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<QuestAssetEditorApp> app = _app.Pin();
    FPropertyEditorModule& propertyEditorModule = 
        FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

    // 에디터 디테일 창에 대한 설정
    FDetailsViewArgs detailsViewArgs{};
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

    // 디테일 창이 어떻게 보일지 정의
    TSharedPtr<IDetailsView> detailsView = propertyEditorModule.CreateDetailView(detailsViewArgs);
    detailsView->SetObject(app->GetWorkingAsset());
    TSharedPtr<IDetailsView> selectedNodeDetailView = propertyEditorModule.CreateDetailView(detailsViewArgs);
    selectedNodeDetailView->SetObject(nullptr);
    app->SetSelectedNodeDetailView(selectedNodeDetailView);
    return SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        .HAlign(HAlign_Fill)
        [
            detailsView.ToSharedRef()
        ]
        +SVerticalBox::Slot()
        .FillHeight(1.0f)
        .HAlign(HAlign_Fill)
        [
            selectedNodeDetailView.ToSharedRef()
        ];
  
}

FText QuestAssetPropertiesTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("a Properties view for the current asset"));
}
