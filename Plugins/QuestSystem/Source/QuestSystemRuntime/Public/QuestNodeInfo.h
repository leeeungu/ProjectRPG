#pragma once

#include "CoreMinimal.h"
#include "QuestNodeInfoBase.h"
#include "QuestNodeInfo.generated.h"

UCLASS(BlueprintType)
class QUESTSYSTEMRUNTIME_API UQuestNodeInfo : public UQuestNodeInfoBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestNodeInfo")
	FText Title{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestNodeInfo")
	FText QuestText{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestNodeInfo")
	TArray<FText> QuestResponse{};
};
