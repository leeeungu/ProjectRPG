#include "C_QuickSlotManagerComponent.h"
#include <GameFramework/PlayerController.h>
#include <C_InventoryComponent.h>
#include <C_GameAlertSubsystem.h>

UC_QuickSlotManagerComponent::UC_QuickSlotManagerComponent() :
	UActorComponent{}
{
	PrimaryComponentTick.bCanEverTick = false;
	for (int i = 0; i < (uint8)E_QuickSlotType::E_QuickSlot_MAX; i++)
	{
		m_arrQuickSlotItem[i] = UC_ItemDataSubsystem::getUnValidItemID_CPP();
	}
}

//bool UC_QuickSlotManagerComponent::useQuickSlot(E_QuickSlotType QuickSlotType, int& useItemID, int nCount)
//{
//	if (!getQuickSlotItemID(QuickSlotType, useItemID))
//		return false;
//
//	bool bResult = m_pInventoryComponent->removeItem(useItemID, nCount);
//	int nRemainCount{};
//	if (bResult && m_onQuickSlotNoneDelegate.IsBound()  && (!m_pInventoryComponent->getItemCountByID(useItemID, nRemainCount) || nRemainCount <= 0))
//	{
//		m_onQuickSlotNoneDelegate.Broadcast();
//	}
//	return bResult;
//}

void UC_QuickSlotManagerComponent::setQuickSlotItem(E_QuickSlotType QuickSlotType, int ItemID)
{
	m_arrQuickSlotItem[(int)QuickSlotType] = ItemID;
}

bool UC_QuickSlotManagerComponent::getQuickSlotItemID(E_QuickSlotType QuickSlotType, int& useItemID) const
{
	useItemID = UC_ItemDataSubsystem::getUnValidItemID_CPP();
	if (!m_pInventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UC_QuickSlotManagerComponent::useQuickSlot - m_pInventoryComponent is null"));
		return false;
	}
	if (QuickSlotType <= E_QuickSlotType::E_None || QuickSlotType >= E_QuickSlotType::E_QuickSlot_MAX)
	{
		UE_LOG(LogTemp, Warning, TEXT("UC_QuickSlotManagerComponent::useQuickSlot - Invalid QuickSlotType: %d"), (int)QuickSlotType);
		return false;
	}

	int ItemID = m_arrQuickSlotItem[(int)QuickSlotType];
	useItemID = ItemID;

	if (useItemID == UC_ItemDataSubsystem::getUnValidItemID_CPP())
	{
		FS_GameAlertSubsystemConfig config{};
		config.strDefaultAlertMessage = FText::FromString(TEXT("퀵슬롯에 아이템이 없습니다."));
		UC_GameAlertSubsystem::pushAlertMessage_Cpp(config);
		return false;
	}
	return true;
}

void UC_QuickSlotManagerComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
	m_pInventoryComponent = GetOwner()->GetComponentByClass<UC_InventoryComponent>();
}