#include "C_InventoryComponent.h"
//#include <queue>

UC_InventoryComponent::UC_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


int UC_InventoryComponent::getItemID(int nY, int nX) 
{
	if (!isBound(nY, nX))
		return 0;
	return m_arrInventory[getArrayIndex(nY , nX)];
}

void UC_InventoryComponent::sortInventory()
{
	//std::queue<int>

}

void UC_InventoryComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
	m_arrInventory.Init(0, m_nInventorySize);
}

void UC_InventoryComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UActorComponent::PostEditChangeProperty(PropertyChangedEvent);
	m_nInventorySize = m_nWidthSize* m_nHeightSize;
}

bool UC_InventoryComponent::isBound(int nY, int nX) 
{
	if (nY < 0 ||  nY >= m_nHeightSize || nX < 0 || nX >= m_nWidthSize)
		return false;
	return true;
}

void UC_InventoryComponent::setItemID(int nY, int nX, int nVal)
{
	if (!isBound(nY, nX))
		return;
	int & rData = m_arrInventory[getArrayIndex(nY, nX)];
	rData = nVal;
}

