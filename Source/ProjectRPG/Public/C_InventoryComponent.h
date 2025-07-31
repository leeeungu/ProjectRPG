#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <C_ItemDataSubsystem.h>
#include "C_InventorySlotInterface.h"
#include "C_InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPushItem, int, nItemID, int, nItemCount);
USTRUCT()
struct FS_InventorySlot
{
	GENERATED_USTRUCT_BODY()
	FS_InventorySlotData sData;
	UPROPERTY()
	TScriptInterface< IC_InventorySlotInterface> pSlotInterface;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "UC_InventoryComponent")
	FOnPushItem m_onPushItem;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UC_InventoryComponent", meta = (ClampMin = 1))
	int m_nInventoryWidth = 1;
	UPROPERTY(EditDefaultsOnly, Category = "UC_InventoryComponent", meta = (ClampMin = 1))
	int m_nInventoryHeight= 1;

private:
	UPROPERTY(VisibleAnywhere, Category = "UC_InventoryComponent")
	int m_nInventorySize;

	UPROPERTY()
	TArray<FS_InventorySlot> m_arrInventory;
	FS_InventorySlot m_sDummyItemData;

	TMap<int, int > m_mapItemCount;
	UC_ItemDataSubsystem* m_pItemDataSubsystem;
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
	int getInventoryWidth()  { return m_nInventoryWidth; }
	/**
	 * @return - Inventory Inventory Size
	 */
	UFUNCTION(BlueprintPure, Category = "UC_InventoryComponent")
	int getInventorySize() { return m_nInventorySize; }
	/**
	 * @return - Inventory MaxHeight
	 */
	UFUNCTION(BlueprintPure, Category = "UC_InventoryComponent")
	int getInventoryHeight()  { return m_nInventoryHeight; }
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
	/**
	* Blueprint getInventorySlotData 
	* @param nY - Inventory Height/Row Index
	* @param nX - Inventory Width/Col Index
	* @param sData - ItemData Structure to fill
	*/
	UFUNCTION(BlueprintCallable, Category = "UC_InventoryComponent") 
	void getInventorySlotData(int nY, int nX, FS_InventorySlotData& sData) ;
	/**
	* @param nSrcY - Item1 Height/Row Index
	* @param nSrcX - Item1 Width/Col Index
	* @param nDstY - Item2 Height/Row Index
	* @param nDstX - Item2 Width/Col Index
	*/
	UFUNCTION(BlueprintCallable, Category = "UC_InventoryComponent") 
	void swapInventorySlot(int nSrcY, int nSrcX, int nDstY, int nDstX);
	/**
	* @param nSrcY - Item1 Height/Row Index
	* @param nSrcX - Item1 Width/Col Index
	* @param nDstY - Item2 Height/Row Index
	* @param nDstX - Item2 Width/Col Index
	*/
	UFUNCTION(BlueprintCallable, Category = "UC_InventoryComponent") 
	bool pushItem(int nItemID, int nItemCount);

	UFUNCTION(BlueprintCallable, Category = "UC_InventoryComponent")
	void setItemSlotLock(int nY, int nX, bool bLock);

	UFUNCTION(BlueprintCallable, Category = "UC_InventoryComponent")
	bool getItemSlotlock(int nY, int nX)  ;


	UFUNCTION(BlueprintPure, Category = "UC_InventoryComponent")
	bool getItemCountAtSlot(int nY, int nX, int& nCount);

	UFUNCTION(BlueprintPure, Category = "UC_InventoryComponent")
	bool getItemCountByID(int nItemID, int & nCount);

	UFUNCTION(BlueprintCallable, Category = "UC_InventoryComponent")
	bool removeItem(int nItemID, int nCount);

	UFUNCTION(BlueprintCallable, Category = "UC_InventoryComponent")
	bool removeItemAtSlot(int nY, int nX, int nCount);

	void setSlotInterface(int nY, int nX, UObject* pInterface);
protected:
	virtual void BeginPlay() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
private:
	/**
	* Check Array Bound
	* UnUsed
	* @use getInventorySlotData(int nY, int nX)
	*/
	bool isBound(int nY, int nX) const;

	/**
	* getInventorySlotData
	*/
	FS_InventorySlot* getInventorySlotData(int nY, int nX) ;
	/**
	* Calculate Array index
	 * @param nY - Inventory Height/Row Index
	 * @param nX - Inventory Width/Col Index
	 * @return - Array Index
	 */
	int getArrayIndex(int nY, int nX) const;
	void resetItemSlot(FS_InventorySlot* pItemSlot);
	void runSlotChangeInterface(FS_InventorySlot* pItemSlot);

};
