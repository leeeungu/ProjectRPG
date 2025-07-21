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
	FString strItemDescription;
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
	FS_ItemData m_sDummyItemData;
public:
	UC_ItemDataSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "ItemData")
	bool getItemDataByID(int ItemID, FS_ItemData& OutData) const;
};
