#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "C_DetectedInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UC_DetectedInterface : public UInterface
{
	GENERATED_BODY()
};

class IC_DetectedInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool beginDetectedEvent(AActor* pOther);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool endDetectedEvent(AActor* pOther);
};
