#include "QuestGraphNode.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenu.h"
#include "QuestNodeInfo.h"


FText UQuestGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UQuestNodeInfo* nodeInfo = Cast<UQuestNodeInfo>(_NodeInfo);
	if (nodeInfo && nodeInfo->Title.IsEmpty())
	{
		FString questTestStr = nodeInfo->QuestText.ToString();
		if (questTestStr.Len() > 15)
		{
			questTestStr = questTestStr.Left(15) + TEXT("...");
		}
		return FText::FromString(questTestStr);
	}
	return nodeInfo->Title;
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
				if (!node)
					return; 
				UQuestNodeInfo* info = node->GetQuestNodeInfo();
				if (!info)
					return;
				info->QuestResponse.Add(FText::FromString(FString::Printf(TEXT("Response %d"), info->QuestResponse.Num())));
				node->SyncPinWithResponses();
				node->UpdateQuestGraphNodeBase();
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
				if (!node)
					return;
				node->GetGraph()->RemoveNode(node);
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
				if (!node)
					return;
				UQuestNodeInfo* info = node->GetQuestNodeInfo();
				if (info->QuestResponse.Num() <= 1)
					return;

				UEdGraphPin* pin = node->GetPinAt(node->Pins.Num() - 1);
				if (pin->Direction != EEdGraphPinDirection::EGPD_Input)
				{
					node->RemovePin(pin);
					info->QuestResponse.RemoveAt(info->QuestResponse.Num() - 1);
					node->UpdateQuestGraphNodeBase();
				}
			}
		))
	);

	section.AddMenuEntry
	(
		TEXT("RestPinEntry"),
		FText::FromString(TEXT("Delete Rest Response")),
		FText::FromString(TEXT("Deletes the All Response And Add Deault Response")),
		FSlateIcon(TEXT("QuestAssetEditorStyle"), TEXT("QuestAssetEditor.NodeDeletePinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[node]()
			{
				if (!node)
					return;
				int nSize = node->Pins.Num()- 1;
				for (int i = nSize; i >= 1; i--)
				{
					UEdGraphPin* pin = node->GetPinAt(i);
					if (pin && pin->Direction == EEdGraphPinDirection::EGPD_Output)
					{
						node->RemovePin(pin);
					}
				}
				node->GetQuestNodeInfo()->QuestResponse.Reset();
				node->CreateDefaultOutputPins();
				node->UpdateQuestGraphNodeBase();
			}
		)
		)
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

UEdGraphPin* UQuestGraphNode::CreateDefaultInputPin()
{
	return CreateQuestPin(EEdGraphPinDirection::EGPD_Input, TEXT("Display"));
}

void UQuestGraphNode::CreateDefaultOutputPins()
{
	FString defaultResponse = TEXT("Continue");
	CreateQuestPin(EEdGraphPinDirection::EGPD_Output, FName(defaultResponse));
	GetQuestNodeInfo()->QuestResponse.Add(FText::FromString(defaultResponse));
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
