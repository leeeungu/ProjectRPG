#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/C_ItemData.h"
#include "C_EquipComponent.generated.h"

class AC_EquipItem;
class AC_BaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipEvent, AC_EquipItem*, pEquipActor);
UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnEquipTypeEvent, bool, IsEquip, AC_EquipItem*, pEquipActor);

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
class PROJECTRPG_API UC_EquipComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "EquipComponent")
	FOnEquipEvent m_onRegister{};

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "EquipComponent")
	FOnEquipEvent m_onUnRegister{};

	TArray< FS_EquipEventBinding> m_arrEquipEvent[(uint8)E_EquipEffectType::E_EquipTypeMax];
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "EquipComponent")
	TMap<E_EquipEffectType, AC_EquipItem*> m_setEquipData{};
private:
	UPROPERTY()
	AC_BaseCharacter* pPlayer{};
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
	void unRegisterEquip(AC_EquipItem* pItemBase);

protected:
	virtual void BeginPlay() override;

private:
	void effectEuip(AC_EquipItem* pItemBase);
	void unEffectEuip(AC_EquipItem* pItemBase);

	void braodCastEquip(E_EquipEffectType EquipType, bool IsEquip = true);
};
