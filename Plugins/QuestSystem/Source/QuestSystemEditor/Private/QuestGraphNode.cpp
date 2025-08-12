#include "QuestGraphNode.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenu.h"

void UQuestGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& section = Menu->AddSection(TEXT("SectionName"), FText::FromString(TEXT("Quest Node Actions")));

	UQuestGraphNode* node = (UQuestGraphNode*)this;

	section.AddMenuEntry
	(
		TEXT("AddPinEntry"),
		FText::FromString(TEXT("Add Pin")),
		FText::FromString(TEXT("Create a New Pin")),
		FSlateIcon(TEXT("QuestAssetEditorStyle"), TEXT("QuestAssetEditor.NodeAddPinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[node]()
			{
				node->CreateQuestPin(
					EEdGraphPinDirection::EGPD_Output,
					TEXT("Another OutPut")
				);
				node->GetGraph()->NotifyGraphChanged();
				node->GetGraph()->Modify();
			}
		))
	);


	section.AddMenuEntry
	(
		TEXT("DeletePinEntry"),
		FText::FromString(TEXT("Delete Pin")),
		FText::FromString(TEXT("Deletes the lastPin")),
		FSlateIcon(TEXT("QuestAssetEditorStyle"), TEXT("QuestAssetEditor.NodeDeletePinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[node]()
			{
				UEdGraphPin* pin = node->GetPinAt(node->Pins.Num() - 1);
				if (pin->Direction != EEdGraphPinDirection::EGPD_Input)
				{
					node->RemovePin(pin);
					node->GetGraph()->NotifyGraphChanged();
					node->GetGraph()->Modify();
				}
			}
		))
	);

		section.AddMenuEntry
		(
			TEXT("DeleteEntry"),
			FText::FromString(TEXT("Delete Node")),
			FText::FromString(TEXT("Deletes the Node")),
			FSlateIcon(TEXT("QuestAssetEditorStyle"), TEXT("QuestAssetEditor.NodeDeleteNodeIcon")),
			FUIAction(FExecuteAction::CreateLambda(
				[node]()
				{
					node->GetGraph()->RemoveNode(node);
				}
			))
	);
}

UEdGraphPin* UQuestGraphNode::CreateQuestPin(EEdGraphPinDirection direction, FName name)
{
	FName category = (direction == EEdGraphPinDirection::EGPD_Input) ? TEXT("Inputs") : TEXT("Outputs");
	FName subcategory = TEXT("QuestPin");

	UEdGraphPin* pin = CreatePin(
		direction, category, name
	);
	pin->PinType.PinSubCategory = subcategory;
	return pin;
}
