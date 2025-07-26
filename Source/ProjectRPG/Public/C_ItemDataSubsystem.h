#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "C_ItemDataSubsystem.generated.h"

UENUM(BlueprintType)
enum class E_EItemType : uint8
{	
	None			UMETA(DisplayName = "None" , Hidden),
	E_Equip			UMETA(DisplayName = "EquipmentItem"),
	E_BattleItem	UMETA(DisplayName = "BattleItem"),
	E_QuestItem		UMETA(DisplayName = "QuestItem"),
	E_Materialitem	UMETA(DisplayName = "Materialitem"),
};

UENUM(BlueprintType, meta = (Bitflags))
enum class E_EItemState : uint8
{
	None = 0					UMETA(DisplayName = "None", Hidden),
	// 아이템을 분해할 수 있는 상태
	CanDisassemble = 1 << 0     UMETA(DisplayName = "Disassembleable"),		
	// 아이템을 버릴 수 있는 상태
	CanDiscard = 1 << 1         UMETA(DisplayName = "Discardable"),			
	// 아이템을 상점에 판매할 수 있는 상태
	CanSell = 1 << 2            UMETA(DisplayName = "Sellable"),			
	// 아이템을 다른 플레이어와 거래할 수 있는 상태
	CanTrade = 1 << 3           UMETA(DisplayName = "Tradeable"),			
	// 아이템을 사용할 수 있는 상태
	CanUse = 1 << 4             UMETA(DisplayName = "Useable"),	
	// 아이템을 인벤토리에서 겹칠 수 있는 상태
	CanStackable = 1 << 5		UMETA(DisplayName = "Stackable"),
};


USTRUCT(BlueprintType)
struct FS_ItemData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FText strItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int nItemID = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FString strItemDescription = TEXT("아무런 효과 없음");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	UTexture2D* pItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	E_EItemType eItemType = E_EItemType::None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Item Data", meta = (Bitmask, BitmaskEnum = E_EItemState))
	uint8 eltemState;
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
	int getUnValidItemID() const { return -1; }

	UFUNCTION(BlueprintPure, Category = "ItemData")
	bool hasItemStateFlag(int ItemID, UPARAM(meta = (Bitmask, BitmaskEnum = E_EItemState)) int32 Bitmask) const;

private:
	FS_ItemData* getItemDataByID_Internal(int ItemID) const;
};
