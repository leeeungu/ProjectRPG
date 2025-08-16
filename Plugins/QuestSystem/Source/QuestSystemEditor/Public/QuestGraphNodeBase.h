#pragma once

#include "EdGraph/EdGraphNode.h"
#include "QuestNodeInfoBase.h"
#include "QuestGraphNodeBase.generated.h"

class UQuestNodeInfo;

UCLASS()
class UQuestGraphNodeBase :public UEdGraphNode
{

	GENERATED_BODY()
public:
	UQuestGraphNodeBase() = default;

public:
	virtual UEdGraphPin* CreateQuestPin(EEdGraphPinDirection direction, FName name) { return nullptr; }
	virtual UEdGraphPin* CreateDefaultInputPin(){ return nullptr;}
	virtual void CreateDefaultOutputPins() {}
	virtual void InitNodeInfo(UObject* output) {}
	virtual void SetNodeInfo(UQuestNodeInfoBase* nodeinfo)  { }
	virtual UQuestNodeInfoBase* GetNodeInfo() const  { return nullptr; }


	virtual EQuestNodeType GetQuestNodeType() const { return EQuestNodeType::UnKnown; }

	virtual void OnPropertiesChanged() {}


protected:
	void UpdateQuestGraphNodeBase();
};

