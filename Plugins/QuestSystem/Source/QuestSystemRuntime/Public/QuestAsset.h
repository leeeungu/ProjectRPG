#pragma once

#include "CoreMinimal.h"
#include "QuestRuntimeGraph.h"
#include <functional>
#include "QuestAsset.generated.h"

class UQuestRuntimeNode;
class AQuestObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAssetEvent,class UQuestAsset* , QuestAsset);

UCLASS(BlueprintType)
class QUESTSYSTEMRUNTIME_API UQuestAsset : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestAsset")
    FString QuestName = TEXT("Enter Quest Name Here");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestAsset")
    FString QuestDesc = TEXT("Enter Quest Desc");

    UPROPERTY()
    UQuestRuntimeGraph* Graph = nullptr;

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnQuestAssetEvent OnQuestSucceed;
    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnQuestAssetEvent OnQuestFail;
protected:
    UPROPERTY(EditAnywhere)
    TSubclassOf<AQuestObject> cQuestObject;


public:
    UFUNCTION(BlueprintCallable, Category = "QuestAsset")
    UQuestRuntimeNode* GetQuestStartNode();

    void SetPreSaveListener(std::function<void()> onPreSaveListener) { _onPreSaveListener = onPreSaveListener ; }

    virtual void PreSave(FObjectPreSaveContext saveContext) override; 

    UFUNCTION(BlueprintPure)
	TSubclassOf<AQuestObject> GetQuestObjectClass() const { return cQuestObject; }
private:
    std::function<void()> _onPreSaveListener = nullptr;
};
