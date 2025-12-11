#include "QuestAssetAppMode.h"
#include "QuestAssetPrimaryTabFactory.h"
#include "QuestAssetEditorApp.h"
#include "QuestAssetPropertiesTabFactory.h"


QuestAssetAppMode::QuestAssetAppMode(TSharedPtr<QuestAssetEditorApp> app) : FApplicationMode(TEXT("QuestAssetAppMode"))
{
	_app = app;
	_tabs.RegisterFactory(MakeShareable(new QuestAssetPrimaryTabFactory(app)));
	_tabs.RegisterFactory(MakeShareable(new QuestAssetPropertiesTabFactory(app)));
	// 내가 만든 커스텀 에셋은 에디터에서 QuestAssetPrimaryTabFactory 과 QuestAssetPropertiesTabFactory으로 구성되어 있다.
	// 아래의 코드는 해당 탭들이 어떻게 배치될지 정의한다.
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
