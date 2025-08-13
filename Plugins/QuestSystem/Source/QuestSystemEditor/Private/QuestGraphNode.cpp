#include "QuestGraphNode.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenu.h"
#include "QuestNodeInfo.h"


FText UQuestGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (_NodeInfo->Title.IsEmpty())
	{
		FString questTestStr = _NodeInfo->QuestText.ToString();
		if (questTestStr.Len() > 15)
		{
			questTestStr = questTestStr.Left(15) + TEXT("...");
		}
		return FText::FromString(questTestStr);
	}
	return _NodeInfo->Title;
}

void UQuestGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& section = Menu->AddSection(TEXT("SectionName"), FText::FromString(TEXT("Quest Node Actions")));

	UQuestGraphNode* node = (UQuestGraphNode*)this;
	section.AddMenuEntry
	(
		TEXT("AddPinEntry"),
		FText::FromString(TEXT("Add Response")),
		FText::FromString(TEXT("Create a New Response")),
		FSlateIcon(TEXT("QuestAssetEditorStyle"), TEXT("QuestAssetEditor.NodeAddPinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[node]()
			{
				node->GetQuestNodeInfo()->QuestResponse.Add(FText::FromString(TEXT("Response")));
				node->SyncPinWithResponses();
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
		FText::FromString(TEXT("Delete Response")),
		FText::FromString(TEXT("Deletes the last Response")),
		FSlateIcon(TEXT("QuestAssetEditorStyle"), TEXT("QuestAssetEditor.NodeDeletePinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[node]()
			{
				UEdGraphPin* pin = node->GetPinAt(node->Pins.Num() - 1);
				if (pin->Direction != EEdGraphPinDirection::EGPD_Input)
				{
					UQuestNodeInfo* info = node->GetQuestNodeInfo();
					info->QuestResponse.RemoveAt(node->Pins.Num() - 1);
					node->SyncPinWithResponses();
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

	//section.AddMenuEntry
	//(
	//	FUICommandInfo
	//	FText::FromString(TEXT("Delete Pin")),
	//	FText::FromString(TEXT("Deletes the lastPin")),
	//	FSlateIcon(TEXT("QuestAssetEditorStyle"), TEXT("QuestAssetEditor.NodeDeletePinIcon")),
	//	FUIAction(FExecuteAction::CreateLambda(
	//		[node]()
	//		{
	//			UEdGraphPin* pin = node->GetPinAt(node->Pins.Num() - 1);
	//			if (pin->Direction != EEdGraphPinDirection::EGPD_Input)
	//			{
	//				node->RemovePin(pin);
	//				node->GetGraph()->NotifyGraphChanged();
	//				node->GetGraph()->Modify();
	//			}
	//		}
	//	))
	//);
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

UEdGraphPin* UQuestGraphNode::CreateDefaultInputPin()
{
	return CreateQuestPin(EEdGraphPinDirection::EGPD_Input, TEXT("Display"));
}

void UQuestGraphNode::CreateDefaultOutputPins()
{

}

void UQuestGraphNode::SyncPinWithResponses()
{
	UQuestNodeInfo* info = GetQuestNodeInfo();
	int numGraphNodePins = Pins.Num() - 1;
	int numInfoPin = info->QuestResponse.Num();

	while (numGraphNodePins > numInfoPin)
	{
		RemovePinAt(numGraphNodePins - 1 , EEdGraphPinDirection::EGPD_Output);
		numGraphNodePins--;
	}

	while (numGraphNodePins < numInfoPin)
	{
		CreateQuestPin(EEdGraphPinDirection::EGPD_Output, FName(info->QuestResponse[numGraphNodePins].ToString()));
		numGraphNodePins++;
	}

	int index = 1;
	for (const FText& option  : info->QuestResponse)
	{
		GetPinAt(index)->PinName = FName(option.ToString());
		index++;

	}
}
