#pragma once

#include "CoreMinimal.h"
#include "QuestRuntimeGraph.h"
#include <functional>
#include "QuestAsset.generated.h"

UCLASS(BlueprintType)
class QUESTSYSTEMRUNTIME_API UQuestAsset : public UObject
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere)
    FString SomeData = TEXT("Enter Dialog Name Here");

    UPROPERTY(EditAnywhere)
    UQuestRuntimeGraph* Graph = nullptr;


public:
    void SetPreSaveListener(std::function<void()> onPreSaveListener) { _onPreSaveListener = onPreSaveListener ; }

    virtual void PreSave(FObjectPreSaveContext saveContext) override; 

private:
    std::function<void()> _onPreSaveListener = nullptr;
};
