﻿#pragma once


#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class  QuestPrimaryTabFactory : public FWorkflowTabFactory
{
public:
	QuestPrimaryTabFactory(TSharedPtr<class QuestEditorApp> app);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<class QuestEditorApp> _app;

};
