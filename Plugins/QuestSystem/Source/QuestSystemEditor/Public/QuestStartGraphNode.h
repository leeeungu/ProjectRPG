#pragma once

#include "EdGraph/EdGraphNode.h"
#include "QuestGraphNodeBase.h"
#include "QuestStartGraphNode.generated.h"


UCLASS()
class QUESTSYSTEMEDITOR_API UQuestStartGraphNode : public UQuestGraphNodeBase
{
	GENERATED_BODY()
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString("Start"); }
	virtual FLinearColor GetNodeTitleColor() const { return FLinearColor(FColor::Red); }
	virtual bool CanUserDeleteNode() const { return false; }

public:
	virtual UEdGraphPin* CreateQuestPin(EEdGraphPinDirection direction, FName name) override;


	virtual EQuestNodeType GetQuestNodeType() const override { return EQuestNodeType::StartNode; }
};
