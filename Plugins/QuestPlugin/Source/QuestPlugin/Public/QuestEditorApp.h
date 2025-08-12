#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class QuestEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
public:
	 virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	 void InitEditor(const EToolkitMode::Type mode, const TSharedPtr<IToolkitHost>& initToolkitHostm, UObject* inObject);


	 class UQuestAsset* GetWorkingAsset() { return _workingAsset; }
public:
	//virtual FName GetToolkitFName() const override = 0;				// Must implement in derived class!
	virtual FName GetToolkitFName() const override { return FName(TEXT("QuestAssetEditorApp")); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("QuestAssetEditorApp")); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("QuestAssetEditorApp"); }
	FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor{ 0.3f,0.2f,0.5f, 0.5f }; }
	virtual FString GetDocumentationLink() const override { return ""; }
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) {}
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) {}

private:
	UPROPERTY()
	class UQuestAsset* _workingAsset = nullptr;
};
