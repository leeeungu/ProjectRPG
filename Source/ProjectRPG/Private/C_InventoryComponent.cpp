#include "C_InventoryComponent.h"
#include <Engine/GameInstance.h>
#include <Kismet/DataTableFunctionLibrary.h>
//#include <queue>

UC_InventoryComponent::UC_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


int UC_InventoryComponent::getItemID(int nY, int nX) 
{
	FS_InventorySlotData* pSlotData = getInventorySlotData(nY, nX);
	if (!pSlotData)
		return -1;
	return pSlotData->nItemID;
}

void UC_InventoryComponent::sortInventoryByItemID()
{
	UDataTable* pDataTable{};//GetGameInstance()->GetSubsystem<UDataTableFunctionLibrary>()->GetInventoryDataTable();
	//pDataTable->GetAllRows<FS_InventorySlotData>("", m_arrInventory);
}

void UC_InventoryComponent::setItemID(int nY, int nX, int nVal)
{
	FS_InventorySlotData* pSlotData = getInventorySlotData(nY, nX);
	if (!pSlotData)
		return;
	pSlotData->nItemID = nVal;
}

void UC_InventoryComponent::getInventorySlotData(int nY, int nX, FS_InventorySlotData& sData)
{
	FS_InventorySlotData* pSlotData = getInventorySlotData(nY, nX);
	if (!pSlotData)
		return;
	sData = *pSlotData;
}

void UC_InventoryComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
//	m_arrInventory.Init(FS_InventorySlotData{}, m_nInventorySize);
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

bool UC_InventoryComponent::isBound(int nY, int nX)  const
{
	if (nY < 0 ||  nY >= m_nInventoryHeight || nX < 0 || nX >= m_nInventoryWidth)
		return false;
	return true;
}

FS_InventorySlotData* UC_InventoryComponent::getInventorySlotData(int nY, int nX) 
{
	if (nY < 0 || nY >= m_nInventoryHeight || nX < 0 || nX >= m_nInventoryWidth)
		return nullptr;
	return &m_arrInventory[getArrayIndex(nY, nX)];
}

int UC_InventoryComponent::getArrayIndex(int nY, int nX) const
{ 
	return nY * m_nInventoryHeight + nX; 
}


