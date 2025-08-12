#pragma once

#include "CoreMinimal.h"
#include "QuestRuntimeGraph.h"
#include "QuestAsset.generated.h"

UCLASS(BlueprintType)
class QUESTSYSTEMRUNTIME_API UQuestAsset : public UObject
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere)
    FString SomeData = TEXT("DefaultData");

    UPROPERTY(EditAnywhere)
    int32 SomeNumber = 0;

    UPROPERTY(EditAnywhere)
    bool SomeBool = false;

    UPROPERTY(EditAnywhere)
    UQuestRuntimeGraph* Graph = nullptr;
};
