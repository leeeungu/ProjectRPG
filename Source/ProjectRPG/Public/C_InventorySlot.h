#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_InventoryComponent.h"
#include "C_InventorySlotInterface.h"
#include "C_ItemDataSubsystem.h"
#include "C_InventorySlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventorySlotChangeEvent, bool, bIsValidID, FS_InventorySlotData, sData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemDataChangeEvent, bool, bIsValidID, FS_ItemData, sData);

UCLASS()
class PROJECTRPG_API UC_InventorySlot : public UUserWidget, public IC_InventorySlotInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UC_InventorySlot")
	FOnItemDataChangeEvent m_onItemDataChangeEvent;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UC_InventorySlot")
	FOnInventorySlotChangeEvent m_onSlotChangeEvent;
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UC_InventoryComponent* m_pInventory{};
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FS_InventorySlotData m_sInventorySlot{};

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FS_ItemData m_sItemData{};
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int m_nIndexX{};
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int m_nIndexY{};
public:
	UC_InventorySlot(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void slotChangeEvent(FS_InventorySlotData* sData) override;

	UFUNCTION(BlueprintCallable, Category = "UC_InventorySlot")
	void setIndex(int nIndexY, int nIndexX);
protected:
	virtual void NativeOnInitialized() override;
};
