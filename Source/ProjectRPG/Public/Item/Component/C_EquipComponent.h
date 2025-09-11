#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/C_ItemData.h"
#include "GamePlay/C_DataManagerInterface.h"
#include "C_EquipComponent.generated.h"

class AC_EquipItem;
class AC_BaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipEvent, int, EquipItem);
UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnEquipTypeEvent, bool, IsEquip, int, EquipItem);

USTRUCT(BlueprintType)
struct FS_EquipData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = EquipData)
	int32 nEquipID{};
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = EquipData)
	int32 nEquipIndex{};
};

USTRUCT()
struct FS_EquipEventBinding
{
	GENERATED_USTRUCT_BODY()
public:
	FOnEquipTypeEvent Delegate{};
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_EquipComponent : public UActorComponent, public IC_DataManagerInterface
{
	GENERATED_BODY()
	struct SEquipData
	{
		int arrEquip[(uint8)E_EquipEffectType::E_EquipTypeMax]{};

		friend FArchive& operator<<(FArchive& Ar, SEquipData* Data)
		{
			for (int i = 0; i < (uint8)E_EquipEffectType::E_EquipTypeMax; i++)
			{
				Ar << Data->arrEquip[i];
			}
			return Ar;
		}
	};
public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "EquipComponent")
	FOnEquipEvent m_onRegister{};

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "EquipComponent")
	FOnEquipEvent m_onUnRegister{};

	TArray< FS_EquipEventBinding> m_arrEquipEvent[(uint8)E_EquipEffectType::E_EquipTypeMax];
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "EquipComponent")
	TMap<E_EquipEffectType, int> m_setEquipData{};
private:
	UPROPERTY()
	AC_BaseCharacter* m_pPlayer{};
	FS_EquipData m_sEffectEquip{};
public:	
	UC_EquipComponent();

	UFUNCTION(BlueprintCallable)
	void bindEquipTypeDelegate(E_EquipEffectType EquipType, FOnEquipTypeEvent Delegate);

	UFUNCTION(BlueprintCallable)
	void registerEquip(AC_EquipItem* pItemBase);

	//UFUNCTION(BlueprintCallable)
	//void registerEquip_Test(int32 nEquipID, int32 EquipIndex, float fValue);

	UFUNCTION(BlueprintCallable)
	void unRegisterEquip(E_EquipEffectType ItemType, int ItemID);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	bool getEquipItemID(E_EquipEffectType ItemType, int& ItemID);
protected:
	virtual void BeginPlay() override;

private:
	void effectEuip(AC_EquipItem* pItemBase, int ItemID);
	void unEffectEuip(int ItemID);

	void braodCastEquip(E_EquipEffectType EquipType, bool IsEquip = true);

	// IC_DataManagerInterface을(를) 통해 상속됨
public:
	virtual E_DataType getDataType() override;
	virtual FString getFilePath(E_DataType eType) override;

	virtual void loadBinaryData(TArray<uint8>& arData) override;
	virtual TArray<uint8> getBinaryData() override;
};
