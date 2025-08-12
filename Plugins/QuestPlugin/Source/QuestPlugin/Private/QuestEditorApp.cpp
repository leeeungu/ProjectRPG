#include "QuestEditorApp.h"
#include "QuestAppMode.h"
#include "QuestAsset.h"


void QuestEditorApp::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void QuestEditorApp::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<IToolkitHost>& initToolkitHost, UObject* inObject)
{
	TArray<UObject*> ObjectsToEdit{};
	ObjectsToEdit.Add(inObject);

	_workingAsset = Cast<UQuestAsset>(inObject);

	InitAssetEditor(
		mode, initToolkitHost, TEXT("QuestPlugin"), FTabManager::FLayout::NullLayout, true, true, ObjectsToEdit
	);

	AddApplicationMode(TEXT("QuestAppMode"), MakeShareable(new QuestAppMode(SharedThis(this))));

	SetCurrentMode(TEXT("QuestAppMode"));
}