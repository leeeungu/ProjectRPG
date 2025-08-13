#include "QuestAssetFactory.h"
#include "QuestAsset.h"

UQuestAssetFactory::UQuestAssetFactory(const FObjectInitializer& ObjectInitializer)
{
	SupportedClass = UQuestAsset::StaticClass();
}

UObject* UQuestAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	UQuestAsset* asset = NewObject< UQuestAsset>(InParent, InName, Flags);
	return asset;
}

bool UQuestAssetFactory::CanCreateNew() const
{
	return true;
}
