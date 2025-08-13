#pragma once

#include "CoreMinimal.h"
#include "QuestNodeInfoBase.h"
#include "QuestNodeInfo.generated.h"

UCLASS(BlueprintType)
class QUESTSYSTEMRUNTIME_API UQuestNodeInfo : public UQuestNodeInfoBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FText Title{};
	UPROPERTY(EditAnywhere)
	FText QuestText{};
	UPROPERTY(EditAnywhere)
	TArray<FText> QuestResponse{};
};
