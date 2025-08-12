#include "QuestAssetAppMode.h"
#include "QuestAssetPrimaryTabFactory.h"
#include "QuestAssetEditorApp.h"
#include "QuestAssetPropertiesTabFactory.h"


QuestAssetAppMode::QuestAssetAppMode(TSharedPtr<QuestAssetEditorApp> app) : FApplicationMode(TEXT("QuestAssetAppMode"))
{
	_app = app;
	_tabs.RegisterFactory(MakeShareable(new QuestAssetPrimaryTabFactory(app)));
	_tabs.RegisterFactory(MakeShareable(new QuestAssetPropertiesTabFactory(app)));

	TabLayout = FTabManager::NewLayout("QuestAssetAppMode_LayOut_v1")
		->AddArea(
			FTabManager::NewPrimaryArea()
				->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewSplitter()
						->SetOrientation(Orient_Horizontal)
						->Split
						(
							FTabManager::NewStack()
								->SetSizeCoefficient(0.75)
								->AddTab(FName(TEXT("QuestAssetPrimaryTab")), ETabState::OpenedTab)
						)
						->Split
						(
							FTabManager::NewStack()
								->SetSizeCoefficient(0.25)
								->AddTab(FName(TEXT("QuestAssetPropertiesTab")), ETabState::OpenedTab)
						)
			
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
