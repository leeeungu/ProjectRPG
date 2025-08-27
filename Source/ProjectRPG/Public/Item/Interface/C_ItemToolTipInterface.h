#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "C_ItemToolTipInterface.generated.h"

UINTERFACE(MinimalAPI)
class UC_ItemToolTipInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTRPG_API IC_ItemToolTipInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText getItemName() const;
	virtual FText getItemName_Implementation() const = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText getItemDesc() const;

	virtual FText getItemDesc_Implementation() const = 0;
};
