#include "QuestFactory.h"
#include "QuestAsset.h"

UQuestFactory::UQuestFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = UQuestAsset::StaticClass();
}

UObject* UQuestFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	UQuestAsset* asset = NewObject< UQuestAsset>(InParent, InName, Flags);
	return asset;
}

bool UQuestFactory::CanCreateNew() const
{
	return true;
}
