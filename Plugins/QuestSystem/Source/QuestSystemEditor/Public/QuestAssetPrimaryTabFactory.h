#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class QuestAssetEditorApp;

class QuestAssetPrimaryTabFactory : public FWorkflowTabFactory
{
public:
	QuestAssetPrimaryTabFactory(TSharedPtr<QuestAssetEditorApp> app);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<QuestAssetEditorApp> _app;

};
