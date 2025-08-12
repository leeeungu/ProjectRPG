#include "QuestAssetAppMode.h"
#include "QuestAssetPrimaryTabFactory.h"
#include "QuestAssetEditorApp.h"


QuestAssetAppMode::QuestAssetAppMode(TSharedPtr<QuestAssetEditorApp> app) : FApplicationMode(TEXT("QuestAssetAppMode"))
{
	_app = app;
	_tabs.RegisterFactory(MakeShareable(new QuestAssetPrimaryTabFactory(app)));
	TabLayout = FTabManager::NewLayout("QuestAssetAppMode_LayOut_v1")
		->AddArea(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)->Split
			(FTabManager::NewStack()->AddTab(FName(TEXT("QuestAssetPrimaryTab")), ETabState::OpenedTab)
			)
		);
}

void QuestAssetAppMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<QuestAssetEditorApp> app = _app.Pin();
	app->PushTabFactories(_tabs);
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void QuestAssetAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void QuestAssetAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
