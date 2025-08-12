#include "QuestAssetEditorApp.h"
#include "QuestAsset.h"
#include "QuestAssetAppMode.h"

//void QuestAssetEditorApp::RegisterTabSpawners(const TSharedRef< class FTabManager>& TabManager)
//{
//	FWorkflowCentricApplication::RegisterTabSpawners(TabManager);
//}


void QuestAssetEditorApp::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<IToolkitHost>& initToolkitHost, UObject* inObject)
{
	TArray<UObject*> ObjectsToEdit{};
	ObjectsToEdit.Add(inObject);

	_workingAsset = Cast<UQuestAsset>(inObject);

	InitAssetEditor(
		mode, initToolkitHost, TEXT("QuestSystem"), FTabManager::FLayout::NullLayout, true, true, ObjectsToEdit
	);

	AddApplicationMode(TEXT("QuestAppMode"), MakeShareable(new QuestAssetAppMode(SharedThis(this))));

	SetCurrentMode(TEXT("QuestAppMode"));
}
