#include "C_InventoryComponent.h"
#include <map>

UC_InventoryComponent::UC_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


int UC_InventoryComponent::getItemID(int nY, int nX) 
{
	FS_InventorySlotData* pSlotData = getInventorySlotData(nY, nX);
	return pSlotData->nItemID;
}

void UC_InventoryComponent::sortInventoryByItemID()
{
	std::map<int,int> ItemMap{};
	std::map<int, int>::iterator InsertResult{};
	for (int i = 0; i < m_nInventorySize; i++)
	{
		if (m_arrInventory[i].nItemID >= 0)
		{
			InsertResult = ItemMap.lower_bound(m_arrInventory[i].nItemID);
				// 이미 존재하는 아이템 ID인 경우, 개수를 합산
			if (InsertResult == ItemMap.end())
				ItemMap.insert(InsertResult, { m_arrInventory[i].nItemID , 1 });
			else
				InsertResult->second += 1;
			m_arrInventory[i].nItemID = -1; // 아이템 ID를 -1로 설정하여 해당 슬롯을 비움
			m_arrInventory[i].nItemCount = 0; // 아이템 개수도 0으로 설정
		}
	}

	int nIndex = 0;
	std::map<int, int >::iterator iter = ItemMap.begin();
	while (iter != ItemMap.end() && nIndex < m_nInventorySize)
	{
		while (iter->second > 0 && nIndex < m_nInventorySize)
		{
			m_arrInventory[nIndex].nItemID = iter->first; // 아이템 ID 설정
			m_arrInventory[nIndex].nItemCount = 1; // 아이템 개수 설정
			iter->second--;
			nIndex++;
		}
		iter++;
	}
}

void UC_InventoryComponent::setItemID(int nY, int nX, int nVal)
{
	FS_InventorySlotData* pSlotData = getInventorySlotData(nY, nX);
	if (pSlotData == &m_sDummyItemData)
		return;
	pSlotData->nItemID = nVal;
}

void UC_InventoryComponent::getInventorySlotData(int nY, int nX, FS_InventorySlotData& sData)
{
	FS_InventorySlotData* pSlotData = getInventorySlotData(nY, nX);
	if (pSlotData == &m_sDummyItemData)
		return;
	sData = *pSlotData;
}

void UC_InventoryComponent::swapInventorySlot(int nSrcY, int nSrcX, int nDstY, int nDstX)
{
	FS_InventorySlotData* pSrcSlotData = getInventorySlotData(nSrcY, nSrcX);
	FS_InventorySlotData* pDstSlotData = getInventorySlotData(nDstY, nDstX);
	if (pSrcSlotData == &m_sDummyItemData || pDstSlotData == &m_sDummyItemData)
		return;
	// Swap the data
	int nData = pSrcSlotData->nItemID;
	pSrcSlotData->nItemID = pDstSlotData->nItemID;
	pDstSlotData->nItemID = nData;


	nData = pSrcSlotData->nItemCount;
	pSrcSlotData->nItemCount = pDstSlotData->nItemCount;
	pDstSlotData->nItemCount = nData;
}

void UC_InventoryComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
	if (m_nInventorySize > 0)
		m_arrInventory.Init(FS_InventorySlotData{}, m_nInventorySize);
}

void UC_InventoryComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UActorComponent::PostEditChangeProperty(PropertyChangedEvent);
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
		return &m_sDummyItemData;
	return &m_arrInventory[getArrayIndex(nY, nX)];
}

int UC_InventoryComponent::getArrayIndex(int nY, int nX) const
{ 
	return nY * m_nInventoryHeight + nX; 
}


