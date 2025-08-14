#include "QuestAsset.h"
#include "UObject/ObjectSaveContext.h"
#include "QuestRuntimeGraph.h"
#include "QuestConditionCheckInterface.h"

DEFINE_LOG_CATEGORY_STATIC(QuestAsset, Log, All);

UQuestRuntimeNode* UQuestAsset::GetQuestStartNode()
{
    UQuestRuntimeNode* pNode{};
    UQuestRuntimeGraph* pGraph = Graph;
    if (pGraph)
    {
        int nSize = pGraph->Nodes.Num();
        for (int i = 0; i < nSize && !pNode;  i++)
        {
            UQuestRuntimeNode* Node = pGraph->Nodes[i];
            if (Node && Node->NodeType == EQuestNodeType::StartNode)
            {
                pNode = Node;
                if (Node->OutputPins[0]->Connection)
                    pNode = Node->OutputPins[0]->Connection->Parent;
            }
        }
    }

    if (!pNode)
    {
        UE_LOG(QuestAsset, Error, TEXT("No Start Node In Asset"));
    }
    return pNode;
}

void UQuestAsset::PreSave(FObjectPreSaveContext saveContext)
{
	UObject::PreSave(saveContext);

	if (_onPreSaveListener)
	{
		_onPreSaveListener();
	}
}

bool UQuestAsset::CheckIsQuestCompeleted()
{
    if (!m_pQuestObject || !m_pQuestObject->Implements< UQuestConditionCheckInterface>())
    {
        m_pQuestObject = nullptr;
        UE_LOG(QuestAsset, Error, TEXT("m_pQuestObject Not have UQuestConditionCheckInterface Interface"));
        return false;
    }
    return IQuestConditionCheckInterface::Execute_CheckCondition(m_pQuestObject);
}
