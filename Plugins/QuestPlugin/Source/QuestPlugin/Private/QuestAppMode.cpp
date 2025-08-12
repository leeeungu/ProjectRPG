#include "QuestAppMode.h"
#include "QuestEditorApp.h"
#include "QuestPrimaryTabFactory.h"

QuestAppMode::QuestAppMode(TSharedPtr<class QuestEditorApp> app) :FApplicationMode(TEXT("QuestAppMode"))
{
	_app = app;
	_tabs .RegisterFactory(MakeShareable(new QuestPrimaryTabFactory(app)));
	TabLayout = FTabManager::NewLayout("QuestAppMode_LayOut_v1")
		->AddArea(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)->Split
			(FTabManager::NewStack()->AddTab(FName(TEXT("PrimaryTab")), ETabState::OpenedTab)
			)
		);

}

void QuestAppMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<QuestEditorApp> app = _app.Pin();
	app->PushTabFactories(_tabs);
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void QuestAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void QuestAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
