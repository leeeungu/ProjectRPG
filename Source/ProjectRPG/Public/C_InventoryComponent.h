#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FS_InventorySlotData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadWrite, Category = "S_InventorySlotData")
	int nItemID;
	UPROPERTY(BlueprintReadWrite, Category = "S_InventorySlotData")
	int nItemCount;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UC_InventoryComponent", meta = (ClampMin = 1))
	int m_nInventoryWidth = 1;
	UPROPERTY(EditDefaultsOnly, Category = "UC_InventoryComponent", meta = (ClampMin = 1))
	int m_nInventoryHeight= 1;

private:
	UPROPERTY(VisibleAnywhere, Category = "UC_InventoryComponent")
	int m_nInventorySize;
	UPROPERTY(VisibleAnywhere, Category = "UC_InventoryComponent")
	TArray<FS_InventorySlotData> m_arrInventory;

public:	
	UC_InventoryComponent();

	/**
	 * getItemID atIndex ((nY  * Inventory MaxWidth) + nX)
	 * @param nY - Inventory Height/Row Index
	 * @param nX - Inventory Width/Col Index
	 * @return 지정된 위치의 아이템 ID. 해당 위치가 인벤토리 범위를 벗어나면 -1을 반환합니다.
	 */
	UFUNCTION(BlueprintPure, Category = "UC_InventoryComponent")
	int getItemID(int nY, int nX);
	/**
	 * @return - Inventory MaxWidth
	 */
	UFUNCTION(BlueprintPure, Category = "UC_InventoryComponent")
	int getWidth()  { return m_nInventoryWidth; }
	/**
	 * @return - Inventory MaxHeight
	 */
	UFUNCTION(BlueprintPure, Category = "UC_InventoryComponent")
	int getHeight()  { return m_nInventoryHeight; }
	/**
	 * SortInventoryData By ID
	 */
	UFUNCTION(BlueprintCallable, Category = "UC_InventoryComponent")
	void sortInventoryByItemID();
	/**
	 * setItemID atIndex ((nY  * Inventory MaxWidth) + nX)
	 * @param nY - Inventory Height/Row Index
	 * @param nX - Inventory Width/Col Index
	 * @param nVal - ItemID to set
	 */
	UFUNCTION(BlueprintCallable, Category = "UC_InventoryComponent")
	void setItemID(int nY, int nX, int nVal);
protected:
	virtual void BeginPlay() override;

	virtual void OnRegister() override;
	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
private:
	/**
	* Check Array Bound
	*/
	bool isBound(int nY, int nX) const;
	/**
	* Calculate Array index
	 * @param nY - Inventory Height/Row Index
	 * @param nX - Inventory Width/Col Index
	 * @return - Array Index
	 */
	int getArrayIndex(int nY, int nX);

};
