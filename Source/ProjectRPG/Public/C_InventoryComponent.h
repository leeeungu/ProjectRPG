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
	int m_nWidthSize;
	UPROPERTY(EditDefaultsOnly, Category = "UC_InventoryComponent")
	int m_nHeightSize;

private:
	TArray<int> m_arrInventory;

public:	
	UC_InventoryComponent();


	/**
	 * GetInventoryData
	 * @param nY - slot Height/Row Index
	 * @param nX - slot Width/Col Index
	 */
	UFUNCTION(BlueprintPure, Category = "UC_InventoryComponent")
	int getSlotData(int nY, int nX);
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
protected:
	virtual void BeginPlay() override;


private:
	bool isBound(int nY, int nX);
	int getArrayIndex(int nY, int nX) { return nY * m_nHeightSize + nX; }
	void setArrayData(int nY, int nX, int nVal);
};
