#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_InventorySlot.generated.h"

UCLASS()
class PROJECTRPG_API UC_InventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UC_InventorySlot(const FObjectInitializer& ObjectInitializer);
};
