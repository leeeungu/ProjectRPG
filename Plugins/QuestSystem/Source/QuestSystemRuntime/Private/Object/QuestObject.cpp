#include "Object/QuestObject.h"
#include "QuestAsset.h"

AQuestObject::AQuestObject() : AActor{}
{

}

void AQuestObject::QuestClear(bool bIsQuestClear)
{
	if (!QuestAsset)
		return;
	if (bIsQuestClear)
	{
		if (QuestAsset->OnQuestSucceed.IsBound())
		{
			QuestAsset->OnQuestSucceed.Broadcast(QuestAsset);
		}
	}
	else
	{
		if (QuestAsset->OnQuestFail.IsBound())
		{
			QuestAsset->OnQuestFail.Broadcast(QuestAsset);
		}
	}
	if (OnQuestEnd.IsBound())
	{
		OnQuestEnd.Broadcast(QuestAsset);
	}
	Destroy();
}

void AQuestObject::InitQuest(AActor* pPlayer, UQuestAsset* pQuest)
{
	QuestAsset = pQuest;
	Player = pPlayer;
	IQuestConditionCheckInterface::Execute_InitQuestCondition(this, pPlayer, pQuest);
}

