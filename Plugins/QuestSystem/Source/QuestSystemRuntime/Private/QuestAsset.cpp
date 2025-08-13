#include "QuestAsset.h"
#include "UObject/ObjectSaveContext.h"

void UQuestAsset::PreSave(FObjectPreSaveContext saveContext)
{
	UObject::PreSave(saveContext);

	if (_onPreSaveListener)
	{
		_onPreSaveListener();
	}
}
