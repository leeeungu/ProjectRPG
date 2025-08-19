#include "C_InventorySlot.h"
#include <Components/Image.h>

UC_InventorySlot::UC_InventorySlot(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer), m_onItemDataChangeEvent{}, m_onSlotChangeEvent{}
{
	
}

void UC_InventorySlot::NativeConstruct()
{
	UUserWidget::NativeConstruct();
	if (m_pInventory)
	{
		m_pInventory->getInventorySlotData(m_nIndexY, m_nIndexX, m_sInventorySlot);
		UC_ItemDataSubsystem::getItemDataByID_CPP(m_sInventorySlot.nItemID, m_sItemData);
		if (m_sItemData.nItemID != UC_ItemDataSubsystem::getUnValidItemID_CPP())
		{
			m_onItemDataChangeEvent.Broadcast(m_sItemData.nItemID != UC_ItemDataSubsystem::getUnValidItemID_CPP(), m_sItemData);
			m_onSlotChangeEvent.Broadcast(m_sItemData.nItemID != UC_ItemDataSubsystem::getUnValidItemID_CPP(), m_sInventorySlot);
		}
	}
}

void UC_InventorySlot::slotChangeEvent(FS_InventorySlotData* sData)
{
	m_sInventorySlot = *sData;
	UC_ItemDataSubsystem::getItemDataByID_CPP(m_sInventorySlot.nItemID, m_sItemData);
	bool bIsValidID = m_sInventorySlot.nItemID != UC_ItemDataSubsystem::getUnValidItemID_CPP();
	if (m_onSlotChangeEvent.IsBound())
	{
		m_onSlotChangeEvent.Broadcast(bIsValidID, m_sInventorySlot);
	}
	if (m_onItemDataChangeEvent.IsBound())
	{
		m_onItemDataChangeEvent.Broadcast(bIsValidID, m_sItemData);
	}
}

void UC_InventorySlot::setIndex(int nIndexY, int nIndexX)
{
	m_nIndexY = nIndexY;
	m_nIndexX = nIndexX;
	if (m_pInventory)
	{
		m_pInventory->setSlotInterface(m_nIndexY, m_nIndexX, this);
	}
}

void UC_InventorySlot::NativeOnInitialized()
{
	m_pInventory = GetOwningPlayer()->GetComponentByClass<UC_InventoryComponent>();
	UUserWidget::NativeOnInitialized();
	if (m_pInventory)
	{
		m_pInventory->getInventorySlotData(m_nIndexY, m_nIndexX, m_sInventorySlot);
		UC_ItemDataSubsystem::getItemDataByID_CPP(m_sInventorySlot.nItemID, m_sItemData);
		m_onItemDataChangeEvent.Broadcast(m_sItemData.nItemID != UC_ItemDataSubsystem::getUnValidItemID_CPP() , m_sItemData);
		m_onSlotChangeEvent.Broadcast(m_sItemData.nItemID != UC_ItemDataSubsystem::getUnValidItemID_CPP() , m_sInventorySlot);
	}
}