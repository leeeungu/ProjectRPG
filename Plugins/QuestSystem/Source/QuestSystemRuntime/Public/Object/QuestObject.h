#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Interface/QuestConditionCheckInterface.h"
#include "QuestObject.generated.h"

class AQuestObject;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestEvent, class UQuestAsset*, TargetQuest);

UCLASS(Blueprintable)
class QUESTSYSTEMRUNTIME_API AQuestObject : public AActor, public IQuestConditionCheckInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UQuestAsset* QuestAsset{};
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AActor* Player{};
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UQuestObject")
	FOnQuestEvent OnQuestCleared{};
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UQuestObject")
	FOnQuestEvent OnQuestCanceled{};
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UQuestObject")
	FOnQuestEvent OnQuestEnd{};

public:
	AQuestObject();

	UFUNCTION(BlueprintCallable)
	void QuestClear(bool bIsQuestClear);
	void InitQuest(AActor* pPlayer, UQuestAsset* pQuest);
	UQuestAsset* GetQuestAsset() const { return QuestAsset; }
protected:
	virtual void InitQuestCondition_Implementaion(AActor* pPlayer, UQuestAsset* pQuest) override {}
	virtual void UpdateQuestCondition_Implementaion() override {}
};
