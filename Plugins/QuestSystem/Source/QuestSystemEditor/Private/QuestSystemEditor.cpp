#include "QuestSystemEditor.h"
#include "QuestAssetAction.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"

#define LOCTEXT_NAMESPACE "FQuestSystemEditorModule"

void FQuestSystemEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& assetTool = IAssetTools::Get();
	EAssetTypeCategories::Type assetType = assetTool.RegisterAdvancedAssetCategory(FName("QuestAssets"), FText::FromString("Quest Asset"));
	TSharedPtr<QuestAssetAction> questAction = MakeShareable(new QuestAssetAction(assetType));
	assetTool.RegisterAssetTypeActions(questAction.ToSharedRef());
}
void FQuestSystemEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuestSystemEditorModule, QuestSystemEditor)