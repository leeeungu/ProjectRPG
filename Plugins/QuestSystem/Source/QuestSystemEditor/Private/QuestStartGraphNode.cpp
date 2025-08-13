#include "QuestStartGraphNode.h"

UEdGraphPin* UQuestStartGraphNode::CreateQuestPin(EEdGraphPinDirection direction, FName name)
{
	FName category =  TEXT("Outputs");
	FName subcategory = TEXT("StartPin");

	UEdGraphPin* pin = CreatePin(
		direction, category, name
	);
	pin->PinType.PinSubCategory = subcategory;
	return pin;
}
