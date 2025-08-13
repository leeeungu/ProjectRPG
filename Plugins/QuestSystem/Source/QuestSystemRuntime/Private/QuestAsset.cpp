#include "QuestAsset.h"
#include "UObject/ObjectSaveContext.h"
#include "QuestRuntimeGraph.h"


DEFINE_LOG_CATEGORY_STATIC(QuestAsset, Log, All);

UQuestRuntimeNode* UQuestAsset::GetQuestStartNode()
{
    UQuestRuntimeNode* pNode{};
    UQuestRuntimeGraph* pGraph = Graph;
    if (pGraph)
    {
        for (UQuestRuntimeNode* Node : pGraph->Nodes)
        {
            if (Node && Node->NodeType == EQuestNodeType::StartNode)
            {
                pNode = Node;
                break;
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
