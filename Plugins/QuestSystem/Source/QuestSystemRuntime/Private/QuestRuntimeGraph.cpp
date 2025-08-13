#include "QuestRuntimeGraph.h"
#include "QuestNodeInfo.h"
#include "QuestEndNodeInfo.h"

DEFINE_LOG_CATEGORY_STATIC(QuestRuntimeNode, Log, All);

UQuestNodeInfo* UQuestRuntimeNode::GetChooseNodeInfo(int nIndex)
{
	if (!OutputPins.IsValidIndex(nIndex))
	{
		UE_LOG(QuestRuntimeNode, Error, TEXT("Invalid Response option At index %d"), nIndex);
		return nullptr;
	}
	UQuestRuntimePin* pPin = OutputPins[nIndex];
	UQuestRuntimeNode* pCurrent{};
	if (pPin->Connection)
	{
		pCurrent = OutputPins[nIndex]->Connection->Parent;
	}
	if (!pCurrent)
	{
		UE_LOG(QuestRuntimeNode, Error, TEXT("No Connection"));
		return nullptr;
	}
	return Cast< UQuestNodeInfo>(pCurrent->QuestInfo);
}

UQuestEndNodeInfo* UQuestRuntimeNode::GetEndNodeInfo(int nIndex)
{
	if (!OutputPins.IsValidIndex(nIndex))
	{
		UE_LOG(QuestRuntimeNode, Error, TEXT("Invalid Response option At index %d"), nIndex);
		return nullptr;
	}
	UQuestRuntimePin* pPin = OutputPins[nIndex];
	UQuestRuntimeNode* pCurrent{};
	if (pPin->Connection)
	{
		pCurrent = OutputPins[nIndex]->Connection->Parent;
	}
	if (!pCurrent)
	{
		UE_LOG(QuestRuntimeNode, Error, TEXT("No Connection"));
		return nullptr;
	}
	return Cast< UQuestEndNodeInfo>(pCurrent->QuestInfo);
}
