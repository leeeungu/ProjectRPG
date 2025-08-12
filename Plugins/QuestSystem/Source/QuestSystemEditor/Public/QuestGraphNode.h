#pragma once

#include "EdGraph/EdGraphNode.h"
#include "QuestGraphNode.generated.h"


UCLASS()
class  UQuestGraphNode : public UEdGraphNode
{
	GENERATED_BODY()
public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const { return FText::FromString("QuestName"); }
	virtual FLinearColor GetNodeTitleColor() const { return FLinearColor(FColor::Green); }
	virtual bool CanUserDeleteNode() const { return true; }
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;


	UEdGraphPin* CreateQuestPin(EEdGraphPinDirection direction, FName name);
};
