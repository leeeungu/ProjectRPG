#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class QuestAppMode : public FApplicationMode
{
public:
	QuestAppMode(TSharedPtr<class QuestEditorApp> app);
	
	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

private:
	TWeakPtr<class QuestEditorApp> _app;
	FWorkflowAllowedTabSet _tabs;
};
