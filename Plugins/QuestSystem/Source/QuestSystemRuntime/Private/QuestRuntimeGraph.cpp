#include "QuestRuntimeGraph.h"
#include "QuestNodeInfo.h"
#include "QuestEndNodeInfo.h"

DEFINE_LOG_CATEGORY_STATIC(QuestRuntimeNode, Log, All);

UQuestRuntimeNode* UQuestRuntimeNode::GetNextNode(int nPinIndex)
{
	if (!OutputPins.IsValidIndex(nPinIndex))
	{
		UE_LOG(QuestRuntimeNode, Error, TEXT("Invalid Response option At index %d"), nPinIndex);
		return nullptr;
	}
	UQuestRuntimePin* pPin = OutputPins[nPinIndex];
	UQuestRuntimeNode* pCurrent{};
	if (pPin->Connection)
	{
		pCurrent = OutputPins[nPinIndex]->Connection->Parent;
	}
	if (!pCurrent)
	{
		UE_LOG(QuestRuntimeNode, Error, TEXT("No Connection"));
		return nullptr;
	}
	return pCurrent;
}
