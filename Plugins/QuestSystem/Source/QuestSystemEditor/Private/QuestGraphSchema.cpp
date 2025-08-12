#include "QuestGraphSchema.h"
#include "QuestGraphNode.h"

void UQuestGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FNewNodeAction> newNodeAction
	{
		new FNewNodeAction(
			FText::FromString(TEXT("Nodes")),
			FText::FromString(TEXT("New Nodes")),
			FText::FromString(TEXT("Make A New Node")),
			0
		)
	};

	ContextMenuBuilder.AddAction(newNodeAction);
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

UEdGraphNode* FNewNodeAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UQuestGraphNode* result = NewObject<UQuestGraphNode>(ParentGraph);
	result->CreateNewGuid();
	result->NodePosX = Location.X;
	result->NodePosY = Location.Y;
	/*result->CreatePin(
		EEdGraphPinDirection::EGPD_Input,
		TEXT("Inputs"),
		TEXT("SomeInput")
	);
	result->CreatePin(
		EEdGraphPinDirection::EGPD_Output,
		TEXT("Outputs"),
		TEXT("Output1")
	);
	result->CreatePin(
		EEdGraphPinDirection::EGPD_Output,
		TEXT("Outputs"),
		TEXT("Output2")
	);*/


	UEdGraphPin* inputPin = result->CreateQuestPin(EEdGraphPinDirection::EGPD_Input, TEXT("SomeInput"));
	result->CreateQuestPin(EEdGraphPinDirection::EGPD_Output, TEXT("Output1"));
	result->CreateQuestPin(EEdGraphPinDirection::EGPD_Output, TEXT("Output2"));

	if (FromPin != nullptr)
	{
		FromPin->GetSchema()->TryCreateConnection(FromPin, inputPin);
	}


	ParentGraph->Modify();
	ParentGraph->AddNode(result, true, true);



	return result;
}
