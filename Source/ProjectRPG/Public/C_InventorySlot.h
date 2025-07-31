#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_InventoryComponent.h"
#include "C_InventorySlotInterface.h"
#include "C_InventorySlot.generated.h"

UCLASS()
class PROJECTRPG_API UC_InventorySlot : public UUserWidget, public IC_InventorySlotInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UC_InventoryComponent* m_pInventory;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FS_InventorySlotData m_sInventorySlot;


public:
	UC_InventorySlot(const FObjectInitializer& ObjectInitializer);

	virtual void slotChangeEvent(FS_InventorySlotData* sData) override;

	UFUNCTION(BlueprintPure)
	bool getItemCount(int& nCount) const;
};
