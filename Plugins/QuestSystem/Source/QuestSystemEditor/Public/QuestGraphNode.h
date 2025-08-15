#pragma once

#include "EdGraph/EdGraphNode.h"
#include "QuestGraphNodeBase.h"
#include "QuestNodeInfo.h"
#include "QuestNodeType.h"
#include "QuestGraphNode.generated.h"

class UQuestNodeInfo;

UCLASS()
class  UQuestGraphNode : public UQuestGraphNodeBase
{
	GENERATED_BODY()
public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const { return FLinearColor(FColor::Green); }
	virtual bool CanUserDeleteNode() const { return true; }
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

public:

	virtual UEdGraphPin* CreateQuestPin(EEdGraphPinDirection direction, FName name) override;

	virtual UEdGraphPin* CreateDefaultInputPin() override;
	virtual void CreateDefaultOutputPins() override;

	virtual void InitNodeInfo(UObject* output) { _NodeInfo = NewObject<UQuestNodeInfo>(output); }
	virtual void SetNodeInfo(UQuestNodeInfoBase* nodeinfo) { _NodeInfo = Cast< UQuestNodeInfo>(nodeinfo); }
	virtual UQuestNodeInfoBase* GetNodeInfo() const { return _NodeInfo; }
	virtual UQuestNodeInfo* GetQuestNodeInfo() const { return _NodeInfo; }


	virtual EQuestNodeType GetQuestNodeType() const { return EQuestNodeType::QuestNode; }
	virtual void OnPropertiesChanged()  override { SyncPinWithResponses(); }

public:
	void SyncPinWithResponses();


protected:
	UPROPERTY()
	UQuestNodeInfo* _NodeInfo = nullptr;
};
