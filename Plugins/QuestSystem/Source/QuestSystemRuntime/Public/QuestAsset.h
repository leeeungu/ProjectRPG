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
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestAsset")
    FString QuestName = TEXT("Enter Quest Name Here");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestAsset")
    FString QuestDesc = TEXT("Enter Quest Desc");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestAsset")
    bool bFinishQuest{};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestAsset")
    bool bQuestAccept{};

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

    UFUNCTION(BlueprintCallable, Category = "QuestAsset")
    void SetFinishQuest(bool bValue);
    UFUNCTION(BlueprintPure, Category = "QuestAsset")
    bool GetFinishQuest() const { return bFinishQuest; }


    UFUNCTION(BlueprintCallable, Category = "QuestAsset")
    void SetQuestAccept(bool bValue) { bQuestAccept = bValue; }
    UFUNCTION(BlueprintPure, Category = "QuestAsset")
    bool GetQuestAccept() const { return bQuestAccept; }

    void SetPreSaveListener(std::function<void()> onPreSaveListener) { _onPreSaveListener = onPreSaveListener ; }

    virtual void PreSave(FObjectPreSaveContext saveContext) override; 

    UFUNCTION(BlueprintPure)
	TSubclassOf<AQuestObject> GetQuestObjectClass() const { return cQuestObject; }
private:
    std::function<void()> _onPreSaveListener = nullptr;
};
