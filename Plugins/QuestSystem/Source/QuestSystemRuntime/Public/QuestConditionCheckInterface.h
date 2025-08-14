#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestConditionCheckInterface.generated.h"

UINTERFACE(MinimalAPI)
class UQuestConditionCheckInterface : public UInterface
{
	GENERATED_BODY()
};

class QUESTSYSTEMRUNTIME_API IQuestConditionCheckInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CheckCondition();

	virtual bool CheckCondition_Implementaion() { return true; }
};

