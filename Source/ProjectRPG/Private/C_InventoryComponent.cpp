#include "C_InventoryComponent.h"

UC_InventoryComponent::UC_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


int UC_InventoryComponent::getSlotData(int nY, int nX)
{
	return 120;
	if (!isBound(nY, nX))
		return 0;
	return m_arrInventory[getArrayIndex(nY , nX)];
}

void UC_InventoryComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
}

bool UC_InventoryComponent::isBound(int nY, int nX)
{
	if (nY < 0 ||  nY >= m_nHeightSize || nX < 0 || nX >= m_nWidthSize)
		return false;
	return true;
}

void UC_InventoryComponent::setArrayData(int nY, int nX, int nVal)
{
	if (!isBound(nY, nX))
		return;
	int & rData = m_arrInventory[getArrayIndex(nY, nX)];
	rData = nVal;
}

