#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "QuestNodeInfo.h"
#include "QuestNodeType.h"
#include "QuestRuntimeGraph.generated.h"


UCLASS()
class QUESTSYSTEMRUNTIME_API UQuestRuntimePin : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName PinName;
	UPROPERTY()
	FGuid PinId;
	UPROPERTY()
	UQuestRuntimePin* Connection = nullptr;
};


UCLASS()
class QUESTSYSTEMRUNTIME_API UQuestRuntimeNode : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	EQuestNodeType  NodeType = EQuestNodeType::QuestNode;

	UPROPERTY()
	UQuestRuntimePin* InputPin = nullptr;
	UPROPERTY()
	TArray<UQuestRuntimePin*>  OutputPins;
	UPROPERTY()
	FVector2D Position;

	UPROPERTY()
	UQuestNodeInfo* QuestInfo;

};


UCLASS()
class QUESTSYSTEMRUNTIME_API UQuestRuntimeGraph: public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<UQuestRuntimeNode*>  Nodes;
};

