#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "C_InventorySlotInterface.generated.h"


USTRUCT(BlueprintType)

struct FS_InventorySlotData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadWrite, Category = "S_InventorySlotData")
	int nItemID = -1;
	UPROPERTY(BlueprintReadWrite, Category = "S_InventorySlotData")
	int nItemCount = 0;
	bool bLockSort;

	FS_InventorySlotData& operator=(const FS_InventorySlotData& sSrc) 
	{
		nItemID = sSrc.nItemID;
		nItemCount = sSrc.nItemCount;
		bLockSort = sSrc.bLockSort;
		return *this;
	}
};

UINTERFACE(MinimalAPI)
class UC_InventorySlotInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTRPG_API IC_InventorySlotInterface
{
	GENERATED_BODY()
public:
	virtual void slotChangeEvent(FS_InventorySlotData* sData) {}
};
