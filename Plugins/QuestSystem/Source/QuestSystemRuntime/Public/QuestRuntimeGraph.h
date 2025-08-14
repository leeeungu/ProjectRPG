#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "QuestNodeInfo.h"
#include "QuestNodeType.h"
#include "QuestRuntimeGraph.generated.h"

class UQuestAsset;
class UQuestNodeInfo;
class UQuestEndNodeInfo;

UCLASS(BlueprintType)
class QUESTSYSTEMRUNTIME_API UQuestRuntimePin : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestRuntimePin")
	FName PinName;
	UPROPERTY()
	FGuid PinId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestRuntimePin")
	UQuestRuntimePin* Connection = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestRuntimePin")
	class UQuestRuntimeNode* Parent = nullptr;
};


UCLASS(BlueprintType)
class QUESTSYSTEMRUNTIME_API UQuestRuntimeNode : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestRuntimePin")
	EQuestNodeType  NodeType = EQuestNodeType::QuestNode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestRuntimePin")
	UQuestRuntimePin* InputPin = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestRuntimePin")
	TArray<UQuestRuntimePin*>  OutputPins;
	UPROPERTY()
	FVector2D Position;

	UPROPERTY(BlueprintReadOnly)
	UQuestNodeInfoBase* QuestInfo;


	UFUNCTION(BlueprintCallable)
	UQuestNodeInfo* GetChooseNodeInfo(int nIndex);
	UFUNCTION(BlueprintCallable)
	UQuestEndNodeInfo* GetEndNodeInfo(int nIndex);

	UFUNCTION(BlueprintCallable)
	UQuestRuntimeNode* GetNextNode(int nIndex);

	UFUNCTION(BlueprintCallable)
	bool GetOutputNodeInfo(int nIndex, UQuestNodeInfoBase*& pResultInfo);
};


UCLASS(BlueprintType)
class QUESTSYSTEMRUNTIME_API UQuestRuntimeGraph: public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<UQuestRuntimeNode*>  Nodes;
};

