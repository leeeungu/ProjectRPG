#include "QuestGraphSchema.h"
#include "QuestGraphNode.h"
#include "QuestNodeInfo.h"
#include "QuestStartGraphNode.h"
#include "QuestEndGraphNode.h"

void UQuestGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FNewNodeAction> newNodeAction
	{
		new FNewNodeAction(
			UQuestGraphNode::StaticClass(),
			FText::FromString(TEXT("Nodes")),
			FText::FromString(TEXT("New Nodes")),
			FText::FromString(TEXT("Make A Quest Node")),
			0
		)
	};

	TSharedPtr<FNewNodeAction> newEndNodeAction
	{
		new FNewNodeAction(
			UQuestEndGraphNode::StaticClass(),
			FText::FromString(TEXT("Nodes")),
			FText::FromString(TEXT("New End Nodes")),
			FText::FromString(TEXT("Make A new End Node")),
			0
		)
	};

	ContextMenuBuilder.AddAction(newNodeAction);
	ContextMenuBuilder.AddAction(newEndNodeAction);
}

const FPinConnectionResponse UQuestGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (A == nullptr || B == nullptr)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Need 2 pins"));
	}
	if (A->Direction == B->Direction)
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW ,TEXT("Inputs can only connect outputs"));

	return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
}

void UQuestGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	UQuestStartGraphNode* startNode = NewObject<UQuestStartGraphNode>(&Graph);
	startNode->CreateNewGuid();
	startNode->NodePosX = 0;
	startNode->NodePosY = 0;
	startNode->CreateQuestPin(EEdGraphPinDirection::EGPD_Output, FName(TEXT("Start")));

	Graph.AddNode(startNode, true, true);
	Graph.Modify();
}

bool UQuestGraphSchema::TryDeleteGraph(UEdGraph* GraphToDelete) const
{
	return false;
}

UEdGraphNode* FNewNodeAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UQuestGraphNodeBase* result = NewObject<UQuestGraphNodeBase>(ParentGraph, _classTemplate);
	result->CreateNewGuid();
	result->NodePosX = Location.X;
	result->NodePosY = Location.Y;
	result->InitNodeInfo(result);


	UEdGraphPin* inputPin = result->CreateDefaultInputPin();
	result->CreateDefaultOutputPins();

	if (FromPin != nullptr)
	{
		FromPin->GetSchema()->TryCreateConnection(FromPin, inputPin);
	}

	ParentGraph->Modify();
	ParentGraph->AddNode(result, true, true);
	return result;
}
