#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestConditionCheckInterface.generated.h"

class UQuestAsset;
class AActor;


UINTERFACE(MinimalAPI)
class UQuestConditionCheckInterface : public UInterface
{
	GENERATED_BODY()
};

class QUESTSYSTEMRUNTIME_API IQuestConditionCheckInterface
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateQuestCondition();
	virtual void UpdateQuestCondition_Implementaion() {}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitQuestCondition(AActor* pPlayer, UQuestAsset* pQuest);

	virtual void InitQuestCondition_Implementaion(AActor* pPlayer, UQuestAsset* pQuest) { }
};

