#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_EquipComponent.generated.h"

class AC_ItemActorBase;
class AC_BaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipEvent, int32, EquipID, int32, EquipIndex);

UENUM(BlueprintType)
enum class E_EquipEffectType : uint8
{
	E_Weapon,
	E_Armor,
};

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_EquipComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "EquipComponent")
	FOnEquipEvent m_onRegister{};

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "EquipComponent")
	FOnEquipEvent m_onUnRegister{};
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "EquipComponent")
	TMap< int32, int32> m_mapEquipData{};
private:
	UPROPERTY()
	AC_BaseCharacter* pPlayer{};
	FS_EquipData m_sEffectEquip{};
public:	
	UC_EquipComponent();

	UFUNCTION(BlueprintCallable)
	void registerEquip(AC_ItemActorBase* pItemBase);

	UFUNCTION(BlueprintCallable)
	void registerEquip_Text(int32 nEquipID, int32 EquipIndex);

	UFUNCTION(BlueprintCallable)
	void unRegisterEquip(int32 nEquipID);

protected:
	virtual void BeginPlay() override;

private:
	void effectEuip(int32 EquipID, int32 EquipIndex);
	void unEffectEuip(int32 EquipID, int32 EquipIndex);

	void effectWeapon(float Value);
	void effectArmor(float Value);

	void unEffectWeapon(float Value);
	void unEffectArmor(float Value);
};
