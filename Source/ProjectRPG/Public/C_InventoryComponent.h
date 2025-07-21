#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UC_InventoryComponent")
	int m_nWidthSize = 1;
	UPROPERTY(EditDefaultsOnly, Category = "UC_InventoryComponent")
	int m_nHeightSize = 1;

private:
	UPROPERTY(VisibleAnywhere, Category = "UC_InventoryComponent")
	int m_nInventorySize = 1;
	UPROPERTY(VisibleAnywhere, Category = "UC_InventoryComponent")
	TArray<int> m_arrInventory;

public:	
	UC_InventoryComponent();

	/**
	 * getItemID atIndex ((nY  * Inventory MaxWidth) + nX)
	 * @param nY - Inventory Height/Row Index
	 * @param nX - Inventory Width/Col Index
	 */
	UFUNCTION(BlueprintPure, Category = "UC_InventoryComponent")
	int getItemID(int nY, int nX) const;
	/**
	 * @return - Inventory MaxWidth
	 */
	UFUNCTION(BlueprintPure, Category = "UC_InventoryComponent")
	int getWidth() const { return m_nWidthSize; }
	/**
	 * @return - Inventory MaxHeight
	 */
	UFUNCTION(BlueprintPure, Category = "UC_InventoryComponent")
	int getHeight() const { return m_nHeightSize; }
	/**
	 * SortInventoryData By ID
	 */
	UFUNCTION(BlueprintCallable, Category = "UC_InventoryComponent")
	void sortInventory();

protected:
	virtual void BeginPlay() override;


	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
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
	int getArrayIndex(int nY, int nX) const { return nY * m_nHeightSize + nX; }
	void setItemID(int nY, int nX, int nVal);
};
	