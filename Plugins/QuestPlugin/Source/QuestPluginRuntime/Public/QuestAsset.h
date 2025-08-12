#pragma once

#include "CoreMinimal.h"
#include "QuestAsset.generated.h"

UCLASS(BlueprintType)
class QUESTPLUGINRUNTIME_API UQuestAsset : public UObject
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnyWhere)
    FString Data = TEXT("Test");
};
