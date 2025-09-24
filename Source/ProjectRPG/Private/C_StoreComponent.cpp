#include "C_StoreComponent.h"
#include "C_ItemDataSubsystem.h"
#include "C_CurrencyComponent.h"
#include "C_InventoryComponent.h"
#include "CPP_Player/C_PlayerController.h"
#include "Item/Component/C_EquipComponent.h"

UC_StoreComponent::UC_StoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UC_StoreComponent::getIsStroeOpen() const
{
	return m_bIsStroeOpen;
}

void UC_StoreComponent::BeginPlay()
{
	Super::BeginPlay();
}
bool UC_StoreComponent::buyItem(UC_CurrencyComponent* pCurrency, UC_InventoryComponent* pInventroy, int nIndex)
{
	FS_ItemStoreData& sData = m_arrStoreList[nIndex];
	if (sData.bBuyOnce && sData.bAlreadyBuy)
	{
		return false;
	}
	FS_ItemData CostItem{};
	UC_ItemDataSubsystem::getItemDataByID_CPP(GetWorld(), sData.nCostItemID, CostItem);
	bool bBuy{};
	if (CostItem.eItemType == E_EItemType::E_Currency)
	{
		bBuy = pCurrency->useCurrency(sData.nCostItemID, sData.nCostItemCount);
	}
	else
	{
		bBuy = pInventroy->removeItem(sData.nCostItemID, sData.nCostItemCount);
	}

	if (bBuy)
	{
		if (sData.bBuyOnce)
		{
			sData.bAlreadyBuy = true;
		}
		pInventroy->pushItem(sData.nBuyItemID, sData.nBuyItemCount);
	}
	return bBuy;
}

bool UC_StoreComponent::getStoreData(int nIndex, FS_ItemStoreData& rData) const
{
	if (m_arrStoreList.IsValidIndex(nIndex))
	{
		rData = m_arrStoreList[nIndex];
		return true;
	}
	return false;
}

bool UC_StoreComponent::checkAlreadyHave(int nIndex, APlayerController* Player)
{
	AC_PlayerController* pPlayer = Cast< AC_PlayerController>(Player);
	if (!pPlayer || !m_arrStoreList.IsValidIndex(nIndex))
		return false;
	UC_EquipComponent* Equip = pPlayer->getEquipComponent();
	UC_InventoryComponent* Inventroy = pPlayer->getInventoryComponent();
	if (!Equip || !Inventroy)
		return false;
	FS_ItemStoreData& sData = m_arrStoreList[nIndex];
	E_EquipEffectType ItemType{};
	int nItemID = UC_ItemDataSubsystem::getUnValidItemID_CPP();
	while (ItemType != E_EquipEffectType::E_EquipTypeMax)
	{
		if (Equip->getEquipItemID(ItemType, nItemID) && nItemID == sData.nBuyItemID)
			return true;
		ItemType = E_EquipEffectType((int)ItemType + 1);
	}

	if (Inventroy->getItemCountByID(sData.nBuyItemID, nItemID))
	{
		return true;
	}
	return false;
}

void UC_StoreComponent::setIsStroeOpen(bool Value)
{
	m_bIsStroeOpen = Value;
}
