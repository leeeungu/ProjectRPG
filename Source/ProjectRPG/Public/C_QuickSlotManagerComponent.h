#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GamePlay/C_DataManagerInterface.h"
#include "C_QuickSlotManagerComponent.generated.h"

class UC_InventoryComponent;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuickSlotNone);
UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnQuickSlotChange, int, ItemID);

UENUM(BlueprintType)
enum class E_QuickSlotType : uint8
{
	E_None UMETA(DisplayName = "None", Hidden),
	E_QuickSlot1 UMETA(DisplayName = "Quick Slot 1"),
	E_QuickSlot2 UMETA(DisplayName = "Quick Slot 2"),
	E_QuickSlot3 UMETA(DisplayName = "Quick Slot 3"),
	E_QuickSlot4 UMETA(DisplayName = "Quick Slot 4"),
	E_QuickSlot5 UMETA(DisplayName = "Quick Slot 5"),
	E_QuickSlot_MAX UMETA(Hidden, DisplayName = "Max Quick Slot")
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTRPG_API UC_QuickSlotManagerComponent : public UActorComponent, public IC_DataManagerInterface
{
	GENERATED_BODY()
private:
	struct FS_QuickSlotSaveData
	{
	public:
		int arrQuickSlotItem[(uint8)E_QuickSlotType::E_QuickSlot_MAX]{};


		friend FArchive& operator<<(FArchive& Ar, FS_QuickSlotSaveData* Data)
		{
			for (int i = 0; i < (uint8)E_QuickSlotType::E_QuickSlot_MAX; i++)
			{
				Ar << Data->arrQuickSlotItem[i];
			}
			return Ar;
		}
	};
public:
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnQuickSlotNone m_onQuickSlotNoneDelegate{};
	UPROPERTY()
	FOnQuickSlotChange m_onQuickSlotChange[(uint8)E_QuickSlotType::E_QuickSlot_MAX]{};
private:
	int m_arrQuickSlotItem[(uint8)E_QuickSlotType::E_QuickSlot_MAX]{};
	UC_InventoryComponent* m_pInventoryComponent{};
public:
	UC_QuickSlotManagerComponent();

	//UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	//bool useQuickSlot(E_QuickSlotType QuickSlotType,int& useItemID, int nCount = 1);

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void setQuickSlotItem(E_QuickSlotType QuickSlotType, int ItemID);


	UFUNCTION(BlueprintPure, Category = "QuickSlot")
	bool getQuickSlotItemID(E_QuickSlotType QuickSlotType, int& useItemID) const;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintPure, Category = "QuickSlot")
	int getQuickSlotID(E_QuickSlotType QuickSlotType) const;

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void bindSlotChangeDelegate(E_QuickSlotType QuickSlotType, FOnQuickSlotChange Delegate);


	//UFUNCTION(BlueprintPure, Category = "QuickSlot")
	UFUNCTION()
	void useQuickSlotItemID(E_QuickSlotType QuickSlotType) ;

protected:
	virtual void BeginPlay() override;


public:
	// IC_DataManagerInterface을(를) 통해 상속됨
	virtual E_DataType getDataType() override;
	virtual FString getFilePath(E_DataType eType) override;

	virtual void loadBinaryData(TArray<uint8>& arData) override;
	virtual TArray<uint8> getBinaryData() override;
};
