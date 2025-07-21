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

void UC_InventoryComponent::sortInventoryByItemID()
{
}

void UC_InventoryComponent::setItemID(int nY, int nX, int nVal)
{
	if (!isBound(nY, nX))
		return;
	int& rData = m_arrInventory[getArrayIndex(nY, nX)];
	rData = nVal;
}

void UC_InventoryComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
	m_arrInventory.Init(0, m_nInventorySize);
}

void UC_InventoryComponent::OnRegister()
{
	UActorComponent::OnRegister();
	m_nInventorySize = m_nInventoryHeight * m_nInventoryWidth;
	if (m_nInventorySize <= 0)
	{
		m_nInventoryHeight = 1;
		m_nInventoryWidth = 1;
		m_nInventorySize = m_nInventoryHeight * m_nInventoryWidth;
	}
}

//void UC_InventoryComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//	UActorComponent::PostEditChangeProperty(PropertyChangedEvent);
//	m_nInventorySize = m_nWidthSize* m_nHeightSize;
//}

bool UC_InventoryComponent::isBound(int nY, int nX)  const
{
	if (nY < 0 ||  nY >= m_nInventoryHeight || nX < 0 || nX >= m_nInventoryWidth)
		return false;
	return true;
}

int UC_InventoryComponent::getArrayIndex(int nY, int nX)
{ 
	return nY * m_nInventoryHeight + nX; 
}


