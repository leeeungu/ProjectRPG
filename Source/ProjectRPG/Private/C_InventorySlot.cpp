#include "C_InventorySlot.h"
#include <C_ItemDataSubsystem.h>

UC_InventorySlot::UC_InventorySlot(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UC_InventorySlot::slotChangeEvent(FS_InventorySlotData* sData)
{
	m_sInventorySlot = *sData;
}

bool UC_InventorySlot::getItemCount(int& nCount) const
{
	if (m_sInventorySlot.nItemID == UC_ItemDataSubsystem::getUnValidItemID_CPP())
		return false;
	nCount = m_sInventorySlot.nItemCount;
	return true;
}
