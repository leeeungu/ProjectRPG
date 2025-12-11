#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "C_GPTInterface.generated.h"

UINTERFACE(MinimalAPI)
class UC_GPTInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTRPG_API IC_GPTInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "GPTInterface")
	void OnRequestComplete(class UVaRestRequestJSON* Request);

	UFUNCTION(BlueprintNativeEvent, Category = "GPTInterface")
	void OnRequestFail(class UVaRestRequestJSON* Request);
};
