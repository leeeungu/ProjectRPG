#include "Item/Component/C_EquipComponent.h"
#include "Item/Actor/C_EquipItem.h"
#include "GameFramework/PlayerController.h"
#include "C_BaseCharacter.h"

UC_EquipComponent::UC_EquipComponent() : UActorComponent{}
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UC_EquipComponent::bindEquipTypeDelegate(E_EquipEffectType EquipType, FOnEquipTypeEvent Delegate)
{
	FS_EquipEventBinding Binding{};
	Binding.Delegate = Delegate;
	m_arrEquipEvent[(uint8)EquipType].Add(Binding);
	//m_arrEquipEvent[]
}

void UC_EquipComponent::registerEquip(AC_EquipItem* pItemBase)
{
	if (!pItemBase)
		return;
	AC_EquipItem*& pValue = m_setEquipData.FindOrAdd(pItemBase->getEquipType(), nullptr); // 여기 메모리 터짐
	if (pValue)
	{
		unRegisterEquip(pValue);
	}
	pValue = pItemBase;
	effectEuip(pValue);
	if (m_onRegister.IsBound())
	{
		m_onRegister.Broadcast(pValue);
	}
}

void UC_EquipComponent::unRegisterEquip(AC_EquipItem* pItemBase)
{
	if (!pItemBase)
		return;
	AC_EquipItem** pValue = m_setEquipData.Find(pItemBase->getEquipType());
	if (!pValue || !*pValue)
		return;
	m_setEquipData.Remove(pItemBase->getEquipType());
	unEffectEuip(*pValue);
	if (m_onUnRegister.IsBound())
	{
		m_onUnRegister.Broadcast(*pValue);
	}
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

void UC_EquipComponent::effectEuip(AC_EquipItem* pItemBase)
{
	if (pPlayer && pItemBase)
	{
		pItemBase->effectEquip(pPlayer);
		braodCastEquip(pItemBase->getEquipType());
	}
}

void UC_EquipComponent::unEffectEuip(AC_EquipItem* pItemBase)
{
	if (pPlayer && pItemBase)
	{
		pItemBase->unEffectEquip(pPlayer);
		braodCastEquip(pItemBase->getEquipType(), false);
	}
}

void UC_EquipComponent::braodCastEquip(E_EquipEffectType EquipType, bool IsEquip)
{
	AC_EquipItem** ppItem = m_setEquipData.Find(EquipType);
	if (!ppItem)
		return;

	for (const FS_EquipEventBinding& Binding : m_arrEquipEvent[(uint8)EquipType])
	{
		if (Binding.Delegate.IsBound())
		{
			Binding.Delegate.Execute(IsEquip, *ppItem);
		}

	}
}

