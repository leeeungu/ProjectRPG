#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
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
	UQuestRuntimePin* InputPin = nullptr;
	UPROPERTY()
	TArray<UQuestRuntimePin*>  OutputPins;
	UPROPERTY()
	FVector2D Position;

};


UCLASS()
class QUESTSYSTEMRUNTIME_API UQuestRuntimeGraph: public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<UQuestRuntimeNode*>  Nodes;
};

