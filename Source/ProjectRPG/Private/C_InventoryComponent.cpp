#include "C_InventoryComponent.h"
#include <map>

UC_InventoryComponent::UC_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

int UC_InventoryComponent::getItemID(int nY, int nX) 
{
	FS_InventorySlotData* pSlotData = getInventorySlotData(nY, nX);
	return pSlotData->nItemID;
}

void UC_InventoryComponent::sortInventoryByItemID()
{
	std::map<int,int> ItemMap{};
	std::pair<std::map<int, int>::iterator,bool> InsertResult{};
	for (int i = 0; i < m_nInventorySize; i++)
	{
		if (m_pItemDataSubsystem->isValidItemID(m_arrInventory[i].nItemID) && !m_arrInventory[i].bLockSort)
		{
			InsertResult = ItemMap.insert({ m_arrInventory[i].nItemID, 1 });
				// �̹� �����ϴ� ������ ID�� ���, ������ �ջ�
			if (!InsertResult.second)
				InsertResult.first->second += 1;
			resetItemSlot(&m_arrInventory[i]);
		}
	}

	int nIndex = 0;
	std::map<int, int >::iterator iter = ItemMap.begin();
	while (iter != ItemMap.end() && nIndex < m_nInventorySize)
	{
		while (iter->second > 0 && nIndex < m_nInventorySize)
		{
			if (!m_pItemDataSubsystem->isValidItemID(m_arrInventory[nIndex].nItemID))
			{
				m_arrInventory[nIndex].nItemID = iter->first; // ������ ID ����
				m_arrInventory[nIndex].nItemCount = 1; // ������ ���� ����
				iter->second--;
			}
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
	Swap<FS_InventorySlotData>(*pSrcSlotData,*pDstSlotData);

	//int nData = pSrcSlotData->nItemID;
	//pSrcSlotData->nItemID = pDstSlotData->nItemID;
	//pDstSlotData->nItemID = nData;
	//nData = pSrcSlotData->nItemCount;
	//pSrcSlotData->nItemCount = pDstSlotData->nItemCount;
	//pDstSlotData->nItemCount = nData;
	//bool bData = pSrcSlotData->bLockSort;
	//pSrcSlotData->bLockSort = pDstSlotData->bLockSort;
	//pDstSlotData->bLockSort = bData;
}

bool UC_InventoryComponent::pushItem(int nItemID, int nItemCount)
{
	FS_InventorySlotData* pSlotData = &m_sDummyItemData;
	for (int i = 0; i < m_nInventorySize && pSlotData  == &m_sDummyItemData; i++)
	{
		if (m_arrInventory[i].nItemID < 0)
			pSlotData = &m_arrInventory[i];
	}
	if (pSlotData != &m_sDummyItemData)
	{
		pSlotData->nItemID = nItemID;
		pSlotData->nItemCount = nItemCount;
		if (m_onPushItem.IsBound())
			m_onPushItem.Broadcast(pSlotData->nItemID, pSlotData->nItemCount);
	}
	return  pSlotData != &m_sDummyItemData;
}

void UC_InventoryComponent::setItemSlotLock(int nY, int nX, bool bLock)
{
	FS_InventorySlotData* pSlotData = getInventorySlotData(nY, nX);
	if (pSlotData == &m_sDummyItemData)
		return;
	pSlotData->bLockSort  = bLock;
}

bool UC_InventoryComponent::getItemSlotlock(int nY, int nX)
{
	FS_InventorySlotData* pSlotData = getInventorySlotData(nY, nX);
	if (pSlotData == &m_sDummyItemData)
		return false;
	return pSlotData->bLockSort;
}

void UC_InventoryComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
	if (m_nInventorySize > 0)
		m_arrInventory.Init(FS_InventorySlotData{}, m_nInventorySize);

	if (!GetWorld())
		return;
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
		m_pItemDataSubsystem = GameInstance->GetSubsystem<UC_ItemDataSubsystem>();
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
	return nY * m_nInventoryWidth + nX;
}

void UC_InventoryComponent::resetItemSlot(FS_InventorySlotData* pItemSlot)
{
	pItemSlot->nItemID = m_pItemDataSubsystem->getUnValidItemID(); // ������ ID�� -1�� �����Ͽ� �ش� ������ ���
	pItemSlot->nItemCount = 0; // ������ ������ 0���� ����
	pItemSlot->bLockSort = false;
}


