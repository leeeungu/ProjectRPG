#include "Item/Component/C_EquipComponent.h"
#include "Item/Actor/C_EquipItem.h"
#include "GameFramework/PlayerController.h"
#include "C_BaseCharacter.h"

UC_EquipComponent::UC_EquipComponent() : UActorComponent{}
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UC_EquipComponent::registerEquip(AC_ItemActorBase* pItemBase)
{
	AC_EquipItem* pEquip = Cast< AC_EquipItem>(pItemBase);
	if (pEquip)
	{
		int32 EquipID = pEquip->getEquipID();
		EquipID = 4;
		int32 EquipIndex  = pEquip->getEquipIndex();
		int32* pValue = m_mapEquipData.Find(EquipID);
		if (pValue)
		{
			unRegisterEquip(EquipID);
		}
		m_mapEquipData.Add(EquipID, EquipIndex);
		effectEuip(EquipID, EquipIndex);
		if (m_onRegister.IsBound())
		{
			m_onRegister.Broadcast(EquipID, EquipIndex);
		}
	}
}

void UC_EquipComponent::unRegisterEquip(int32 nEquipID)
{
	int32* pValue = m_mapEquipData.Find(nEquipID);
	if (!pValue)
		return;
	if (m_onUnRegister.IsBound())
	{
		m_onUnRegister.Broadcast(nEquipID, *pValue);
	}
	unEffectEuip(nEquipID, *pValue);
	m_mapEquipData.Remove(nEquipID);
}

void UC_EquipComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
	APlayerController* Controller = Cast<APlayerController>(GetOwner());
	if (Controller)
	{
		pPlayer = Cast<AC_BaseCharacter>(Controller->AcknowledgedPawn);
	}
}

void UC_EquipComponent::effectEuip(int32 EquipID, int32 EquipIndex)
{
	m_sEffectEquip.nEquipID = EquipID;
	m_sEffectEquip.nEquipIndex = EquipIndex;
	if (pPlayer)
	{
		effectWeapon(10);
	}
}

void UC_EquipComponent::unEffectEuip(int32 EquipID, int32 EquipIndex)
{
	m_sEffectEquip.nEquipID = EquipID;
	m_sEffectEquip.nEquipIndex = EquipIndex;
	if (pPlayer)
	{
		unEffectWeapon(10);
	}
}

void UC_EquipComponent::effectWeapon(float Value)
{
	float fAtk = pPlayer->getAtk();
	fAtk = FMath::Clamp(fAtk, fAtk, TNumericLimits<float>::Max() - Value);
	fAtk += Value;
	pPlayer->setAtk(fAtk);
	//getHp();
	//setMaxHp();
	//getMaxHp();
	//setAtk();
}

void UC_EquipComponent::effectArmor(float Value)
{
	float fHp = pPlayer->getHp();
	fHp = FMath::Clamp(fHp, fHp, TNumericLimits<float>::Max() - Value);
	fHp += Value;
	pPlayer->setHp(fHp);

	fHp = pPlayer->getMaxHp();
	fHp = FMath::Clamp(fHp, fHp, TNumericLimits<float>::Max() - Value);
	fHp += Value;
	pPlayer->setMaxHp(fHp);
}

void UC_EquipComponent::unEffectWeapon(float Value)
{
	float fAtk = pPlayer->getAtk();
	fAtk = FMath::Clamp(fAtk, TNumericLimits<float>::Min() + Value, fAtk);
	fAtk -= Value;
	pPlayer->setAtk(fAtk);
}

void UC_EquipComponent::unEffectArmor(float Value)
{
	float fHp = pPlayer->getHp();
	fHp = FMath::Clamp(fHp, TNumericLimits<float>::Min() + Value, fHp);
	fHp -= Value;
	pPlayer->setHp(fHp);

	fHp = pPlayer->getMaxHp();
	fHp = FMath::Clamp(fHp, TNumericLimits<float>::Min() + Value, fHp);
	fHp -= Value;
	pPlayer->setMaxHp(fHp);
}
