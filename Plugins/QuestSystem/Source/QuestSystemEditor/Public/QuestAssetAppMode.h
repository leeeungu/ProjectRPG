#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class QuestAssetEditorApp;

class  QuestAssetAppMode : public FApplicationMode
{
public:
	QuestAssetAppMode(TSharedPtr<QuestAssetEditorApp> app);

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

private:
	TWeakPtr<QuestAssetEditorApp> _app;
	FWorkflowAllowedTabSet _tabs;
};
