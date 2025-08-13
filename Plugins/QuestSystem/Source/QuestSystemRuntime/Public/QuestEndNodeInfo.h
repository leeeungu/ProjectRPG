#pragma once

#include "CoreMinimal.h"
#include "QuestNodeInfoBase.h"
#include "QuestEndNodeInfo.generated.h"

UENUM(BlueprintType)
enum class EQuestNodeAction : uint8 {
    None,
    StartQuest // ActionData is the QuestId
};

UCLASS(BlueprintType)
class QUESTSYSTEMRUNTIME_API UQuestEndNodeInfo : public UQuestNodeInfoBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    EQuestNodeAction Action = EQuestNodeAction::None;

    UPROPERTY(EditAnywhere)
    FString ActionData = TEXT("");
};