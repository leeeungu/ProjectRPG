#include "Item/Component/C_EquipComponent.h"
#include "Item/Actor/C_EquipItem.h"
#include "GameFramework/PlayerController.h"
#include "C_BaseCharacter.h"
#include "C_ItemDataSubsystem.h"
#include "GamePlay/C_DataMangerSubsystem.h"


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
	E_EquipEffectType Type = pItemBase->getEquipType();
	int* pValue = m_setEquipData.Find(Type);
	if (pValue)
	{
		unRegisterEquip(Type , *pValue);
	}
	int ItemID = m_setEquipData.Add(Type, pItemBase->getItemID());
	effectEuip(pItemBase , ItemID);
	if (m_onRegister.IsBound())
	{
		m_onRegister.Broadcast(ItemID);
	}
}

void UC_EquipComponent::unRegisterEquip(E_EquipEffectType ItemType, int ItemID)
{
	const int* pValue = m_setEquipData.Find(ItemType);
	if (!pValue)
		return;
	unEffectEuip(ItemID);
	m_setEquipData.Remove(ItemType);
	if (m_onUnRegister.IsBound())
	{
		m_onUnRegister.Broadcast(ItemID);
	}
}

void UC_EquipComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UActorComponent::EndPlay(EndPlayReason);
	UC_DataMangerSubsystem::saveBinaryData(this);
}

void UC_EquipComponent::braodCastEquip(E_EquipEffectType EquipType, bool IsEquip)
{
	const int* ppItem = m_setEquipData.Find(EquipType);
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

E_DataType UC_EquipComponent::getDataType()
{
	return E_DataType::E_Binary;
}

FString UC_EquipComponent::getFilePath(E_DataType eType)
{
	return  FPaths::ProjectSavedDir() + TEXT("EquipData");
}

void UC_EquipComponent::loadBinaryData(TArray<uint8>& arData)
{
	SEquipData Data{};
	if (!UC_DataMangerSubsystem::readBinaryFile(arData, &Data))
		return;
	for (auto& Equip : Data.arrEquip)
	{
		if (Equip <= 0)
			Equip =  UC_ItemDataSubsystem::getUnValidItemID_CPP();
		if (Equip > 0)
		{
			AC_EquipItem* pItem = Cast< AC_EquipItem>(UC_ItemDataSubsystem::spawnEffectItem_Cpp(Equip, m_pPlayer));
			if (pItem)
			{
				registerEquip(pItem);

			}
		}
	}
}

TArray<uint8> UC_EquipComponent::getBinaryData()
{
	SEquipData Data{};
	TArray<uint8> result{};

	for (auto& Equip : m_setEquipData)
	{
		if (Equip.Value <= 0 )
			Equip.Value = UC_ItemDataSubsystem::getUnValidItemID_CPP();
		Data.arrEquip[(uint8)Equip.Key] = Equip.Value;
	}
	UC_DataMangerSubsystem::saveBinaryFile< SEquipData>(result, &Data);
	return result;
}


bool UC_EquipComponent::getEquipItemID(E_EquipEffectType ItemType, int& ItemID)
{
	ItemID = UC_ItemDataSubsystem::getUnValidItemID_CPP();
	int* pValue = m_setEquipData.Find(ItemType); // 여기 메모리 터짐
	if (pValue)
		ItemID = *pValue;
	return pValue != nullptr;
}

void UC_EquipComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
	APlayerController* Controller = Cast<APlayerController>(GetOwner());
	if (Controller)
	{
		m_pPlayer = Cast<AC_BaseCharacter>(Controller->AcknowledgedPawn);
		UC_DataMangerSubsystem::loadData(this);
	}
}

void UC_EquipComponent::effectEuip(AC_EquipItem* pItemBase, int ItemID)
{
	if (m_pPlayer && pItemBase)
	{
		pItemBase->effectEquip(m_pPlayer);
		braodCastEquip(pItemBase->getEquipType());
	}
	pItemBase->Destroy();
}

void UC_EquipComponent::unEffectEuip(int ItemID)
{
	AC_EquipItem* pItem = Cast< AC_EquipItem>(UC_ItemDataSubsystem::spawnEffectItem_Cpp(ItemID, m_pPlayer));
	if (m_pPlayer && pItem)
	{
		pItem->unEffectEquip(m_pPlayer);
		braodCastEquip(pItem->getEquipType(), false);
	}
	pItem->Destroy();
}

