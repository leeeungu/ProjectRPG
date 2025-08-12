#include "QuestSystemEditor.h"
#include "QuestAssetAction.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "EdGraphUtilities.h"
#include "KismetPins/SGraphPinColor.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "FQuestSystemEditorModule"

class SQuestGraphPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SQuestGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InPin);
	}


protected:
	virtual FSlateColor GetPinColor() const override
	{
		return FSlateColor(FLinearColor(0.2f, 1.0f, 0.2f));
	}
};

struct FQuestPinFactory : public FGraphPanelPinFactory 
{
public:
	virtual ~FQuestPinFactory() {}
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override
	{
		if (FName(TEXT("QuestPin")) == Pin->PinType.PinSubCategory)
		{
			return SNew(SQuestGraphPin, Pin);
		}
		return nullptr;
	}
};

void FQuestSystemEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& assetTool = IAssetTools::Get();
	EAssetTypeCategories::Type assetType = assetTool.RegisterAdvancedAssetCategory(FName("QuestAssets"), FText::FromString("Quest Asset"));
	TSharedPtr<QuestAssetAction> questAction = MakeShareable(new QuestAssetAction(assetType));
	assetTool.RegisterAssetTypeActions(questAction.ToSharedRef());


	_styleSet = MakeShareable(new FSlateStyleSet(TEXT("QuestAssetEditorStyle")));
	TSharedPtr<IPlugin> plugin = IPluginManager::Get().FindPlugin("QuestSystem");
	FString contentDir = plugin->GetContentDir();
	_styleSet->SetContentRoot(contentDir);


	FSlateImageBrush* nodeAddPinIcon = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("NodeAddPinIcon"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	FSlateImageBrush* nodeDeletePinIcon = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("NodeDeletePinIcon"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	FSlateImageBrush* nodeDeleteNodeIcon = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("NodeDeleteNodeIcon"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	_styleSet->Set(TEXT("QuestAssetEditor.NodeAddPinIcon"), nodeAddPinIcon);
	_styleSet->Set(TEXT("QuestAssetEditor.NodeDeletePinIcon"), nodeDeletePinIcon);
	_styleSet->Set(TEXT("QuestAssetEditor.NodeDeleteNodeIcon"), nodeDeleteNodeIcon);
	FSlateStyleRegistry::RegisterSlateStyle(*_styleSet);

	//// Register a custom pin factory for creating pins the way we want
	_pinFactory = MakeShareable(new FQuestPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(_pinFactory);
}
void FQuestSystemEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSlateStyleRegistry::UnRegisterSlateStyle(*_styleSet);
	FEdGraphUtilities::UnregisterVisualPinFactory(_pinFactory);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuestSystemEditorModule, QuestSystemEditor)
