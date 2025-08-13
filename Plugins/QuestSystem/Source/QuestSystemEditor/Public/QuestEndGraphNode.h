#pragma once

#include "EdGraph/EdGraphNode.h"
#include "QuestEndNodeInfo.h"
#include "QuestNodeType.h"
#include "QuestGraphNodeBase.h"
#include "QuestEndGraphNode.generated.h"

class UQuestNodeInfo;

UCLASS()
class UQuestEndGraphNode:public UQuestGraphNodeBase
{

	GENERATED_BODY()
public:
	UQuestEndGraphNode() = default;

public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const { return FLinearColor(FColor::Blue); }
	virtual bool CanUserDeleteNode() const { return true; }
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

public:
	virtual UEdGraphPin* CreateQuestPin(EEdGraphPinDirection direction, FName name) override;
	virtual UEdGraphPin* CreateDefaultInputPin() override;

	virtual void InitNodeInfo(UObject* Outer) override { _nodeInfo = NewObject<UQuestEndNodeInfo>(Outer); }
	virtual void SetNodeInfo(UQuestNodeInfoBase* nodeinfo) override { _nodeInfo = Cast<UQuestEndNodeInfo>(nodeinfo); }
	virtual UQuestNodeInfoBase* GetNodeInfo() const override { return _nodeInfo; }
	virtual EQuestNodeType GetQuestNodeType() const override { return EQuestNodeType::EndNode; }

	virtual void OnPropertiesChanged() { Modify(); }

protected:
	UPROPERTY()
	UQuestEndNodeInfo* _nodeInfo;
};

