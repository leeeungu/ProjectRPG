#include "Quest/C_QuestManager.h"
#include "QuestEndNodeInfo.h"
#include "QuestAsset.h"

DEFINE_LOG_CATEGORY_STATIC(QuestManager, Log, All);

UC_QuestManager::UC_QuestManager() : UActorComponent{}
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UC_QuestManager::BeginPlay()
{
	UActorComponent::BeginPlay();
}

bool UC_QuestManager::pushQuest(UQuestEndNodeInfo* pEndNode)
{
	if(!pEndNode || !pEndNode->pQuestAsset)
	{
		UE_LOG(QuestManager, Error, TEXT("UC_QuestManager : UQuestEndNodeInfo or UQuestEndNodeInfo's QuestAsset is nullptr"));
		return false;
	}
	if(pEndNode->Action == EQuestNodeAction::None)
		return false;

	m_psetQuest.Add(pEndNode->pQuestAsset);
	return true;
}
