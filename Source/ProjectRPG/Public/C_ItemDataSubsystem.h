#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "C_ItemDataSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FS_ItemData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FText strItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int nItemID = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FString strItemDescription = "아무런 효과 없음";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	UTexture2D* pItemIcon;
};


UCLASS()
class PROJECTRPG_API UC_ItemDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item|Data")
	UDataTable* m_pItemDataTable;
	FString m_strDataTablePath = TEXT("/Game/Item/DataTable/DT_ItemData.DT_ItemData");
private:
	TMap<int, const FS_ItemData*> m_mapItemData;
public:
	UC_ItemDataSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintPure, Category = "ItemData")
	bool getItemDataByID(int ItemID, FS_ItemData& OutData) const;
	UFUNCTION(BlueprintPure, Category = "ItemData")
	bool isValidItemID(int ItemID) const;

	UFUNCTION(BlueprintPure, Category = "ItemData")
	int getUnValidItemID() { return -1; }
};
