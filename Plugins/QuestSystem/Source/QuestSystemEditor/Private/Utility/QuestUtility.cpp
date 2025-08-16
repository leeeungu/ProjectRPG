#include "Utility/QuestUtility.h"
#include "QuestStartGraphNode.h"
#include "QuestGraphNode.h"
#include "QuestEndGraphNode.h"

DEFINE_LOG_CATEGORY_STATIC(QuestUtil, Log, All);

bool QuestUtility::CreateQuestGraphNodeBase(UObject* Outer, EQuestNodeType eTyp, UQuestGraphNodeBase*& rResult)
{
    rResult = nullptr;
    switch (eTyp)
    {
    case EQuestNodeType::StartNode:
        rResult = NewObject<UQuestStartGraphNode>(Outer);
        break;
    case EQuestNodeType::QuestNode:
        rResult = NewObject<UQuestGraphNode>(Outer);
        break;
    case EQuestNodeType::EndNode:
        rResult = NewObject<UQuestEndGraphNode>(Outer);
        break;
    }

    if (rResult == nullptr)
    {
        UE_LOG(QuestUtil, Error, TEXT("QuestUtility::CreateQuestGraphNodeBase : Add New UQuestGraphNodeBase class or Check EQuestNodeType"));
    }
    return rResult != nullptr;
}
