#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "C_SearchingInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UC_SearchingInterface : public UInterface
{
	GENERATED_BODY()
};

class IC_SearchingInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool beginSearchingEvent(AActor* pOther);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool endSearchingEvent(AActor* pOther);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool runEvent(AActor* pOther);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool endEvent();
};
