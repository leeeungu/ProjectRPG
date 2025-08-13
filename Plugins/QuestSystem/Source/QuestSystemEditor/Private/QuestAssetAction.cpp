#include "QuestAssetAction.h"
#include "QuestAsset.h"
#include "QuestAssetEditorApp.h"

QuestAssetAction::QuestAssetAction(EAssetTypeCategories::Type categpry)
{
	_assetCategpry = categpry;
}

FText QuestAssetAction::GetName() const
{
	return FText::FromString(TEXT("Quest Asset"));
}

FColor QuestAssetAction::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* QuestAssetAction::GetSupportedClass() const
{
	return UQuestAsset::StaticClass();
}

void QuestAssetAction::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* object : InObjects)
	{
		UQuestAsset* questAsset = Cast< UQuestAsset>(object);
		if (questAsset != nullptr)
		{
			TSharedRef<QuestAssetEditorApp> editor(new QuestAssetEditorApp());
			editor->InitEditor(mode, EditWithinLevelEditor, questAsset);
		}
	}
}

uint32 QuestAssetAction::GetCategories()
{
	return _assetCategpry;
}
