#pragma once

#include "CoreMinimal.h"
#include "QuestRuntimeGraph.h"
#include <functional>
#include "QuestAsset.generated.h"

class UQuestRuntimeNode;

UCLASS(BlueprintType)
class QUESTSYSTEMRUNTIME_API UQuestAsset : public UObject
{
    GENERATED_BODY()
public:


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestAsset")
    FString SomeData = TEXT("Enter Dialog Name Here");

    UPROPERTY(EditAnywhere)
    UQuestRuntimeGraph* Graph = nullptr;

    UPROPERTY(EditAnywhere)
    UObject* m_pQuestObject;
public:
    UFUNCTION(BlueprintCallable, Category = "QuestAsset")
    UQuestRuntimeNode* GetQuestStartNode();


    void SetPreSaveListener(std::function<void()> onPreSaveListener) { _onPreSaveListener = onPreSaveListener ; }

    virtual void PreSave(FObjectPreSaveContext saveContext) override; 


    UFUNCTION(BlueprintCallable)
    bool CheckIsQuestCompeleted();
private:
    std::function<void()> _onPreSaveListener = nullptr;
};
