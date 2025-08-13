#include "QuestEndGraphNode.h"


FText UQuestEndGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    if (_nodeInfo != nullptr && _nodeInfo->Action != EQuestNodeAction::None) {
        FString result = UEnum::GetDisplayValueAsText(_nodeInfo->Action).ToString();
        if (!_nodeInfo->ActionData.IsEmpty()) {
            FString actionData = _nodeInfo->ActionData;
            if (actionData.Len() > 15) {
                actionData = actionData.Left(15) + TEXT("...");
            }
            result += TEXT(" - ") + actionData;
        }
        return FText::FromString(result);
    }
    return FText::FromString(TEXT("End"));
}

void UQuestEndGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& section = Menu->AddSection(TEXT("SectionName"), FText::FromString(TEXT("End Node Actions")));

	UQuestEndGraphNode* node = (UQuestEndGraphNode*)this;
	section.AddMenuEntry
	(
		TEXT("DeleteEntry"),
		FText::FromString(TEXT("Delete Node")),
		FText::FromString(TEXT("Deletes a New Node")),
		FSlateIcon(TEXT("QuestAssetEditorStyle"), TEXT("QuestAssetEditor.NodedeleteNodeIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[node]()
			{
				node->GetGraph()->RemoveNode(node);
			}
		))
	);

}

UEdGraphPin* UQuestEndGraphNode::CreateQuestPin(EEdGraphPinDirection direction, FName name)
{
	FName category = TEXT("Inputs");
	FName subcategory = TEXT("EndPin");

	UEdGraphPin* pin = CreatePin(
		direction, category, name
	);
	pin->PinType.PinSubCategory = subcategory;
	return pin;
}

UEdGraphPin* UQuestEndGraphNode::CreateDefaultInputPin()
{
	return CreateQuestPin(EEdGraphPinDirection::EGPD_Input, FName(TEXT("Finish")));
}

