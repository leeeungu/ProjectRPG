#include "QuestRuntimeGraph.h"
#include "QuestNodeInfo.h"
#include "QuestEndNodeInfo.h"

DEFINE_LOG_CATEGORY_STATIC(QuestRuntimeNode, Log, All);

UQuestNodeInfo* UQuestRuntimeNode::GetChooseNodeInfo(int nIndex)
{
	UQuestRuntimeNode* pCurrent = GetNextNode(nIndex);
	if (!pCurrent)
		return nullptr;
	return Cast< UQuestNodeInfo>(pCurrent->QuestInfo);
}

UQuestEndNodeInfo* UQuestRuntimeNode::GetEndNodeInfo(int nIndex)
{
	UQuestRuntimeNode* pCurrent = GetNextNode(nIndex);
	if (!pCurrent)
		return nullptr;
	return Cast< UQuestEndNodeInfo>(pCurrent->QuestInfo);
}

UQuestRuntimeNode* UQuestRuntimeNode::GetNextNode(int nIndex)
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
	return pCurrent;
}

bool UQuestRuntimeNode::GetOutputNodeInfo(int nIndex, UQuestNodeInfoBase*& pResultInfo)
{
	UQuestRuntimeNode* pCurrent = GetNextNode(nIndex);
	pResultInfo = nullptr;
	if (!pCurrent)
		return false;
	if (pCurrent->NodeType == EQuestNodeType::UnKnown)
		return false;
	pResultInfo = pCurrent->QuestInfo;
	return true;
}

