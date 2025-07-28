#include "C_InventoryComponent.h"
#include <map>
#include <queue>
#include <C_GameAlertSubsystem.h>

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
	using namespace std;
	struct SortFunc
	{	
		bool operator()(const pair<int,int>& a, const pair<int, int>& b) const
		{
			return a.first > b.first;
		}
	};
	priority_queue<pair<int, int>, vector<pair<int, int>>, SortFunc> ItemQueue{};
	// Reset the inventory slots that are not locked
	for (int i = 0; i < m_nInventorySize; i++)
	{
		if (m_pItemDataSubsystem->isValidItemID(m_arrInventory[i].nItemID) && !m_arrInventory[i].bLockSort)
		{
			ItemQueue.push({ m_arrInventory[i].nItemID, m_arrInventory[i].nItemCount });
			resetItemSlot(&m_arrInventory[i]);
		}
	}

	int NextIndex = 0; // 다음 아이템이 삽입될 인덱스
	int CurrentIndex = 0; // 현재 아이템이 삽입 되는 인덱스
	while (!ItemQueue.empty() && CurrentIndex < m_nInventorySize)
	{
		// 다음 인덱스가 유효한지 확인하고, 아이템이 유효하지 않거나 잠금 상태가 아닌 경우에만 진행
		while (NextIndex < m_nInventorySize && 
			m_arrInventory[NextIndex].nItemID != m_pItemDataSubsystem->getUnValidItemID() &&
			m_arrInventory[NextIndex].bLockSort)
		{
			NextIndex++;
		}
		pair<int, int> item = ItemQueue.top();
		ItemQueue.pop();
		
		// Check if the current item can be stacked with the existing item at CurrentIndex
		if (item.first == m_arrInventory[CurrentIndex].nItemID)
		{
			// 같은 아이템 이고 스택 가능 할때
			if (m_pItemDataSubsystem->hasItemStateFlag(item.first, (int32)E_EItemState::CanStackable))
			{
				m_arrInventory[CurrentIndex].nItemCount += item.second;
			} 
			else // 같은 아이템 이지만 스택 불가능 할때
			{
				CurrentIndex = NextIndex;
				m_arrInventory[CurrentIndex].nItemID = item.first;
				m_arrInventory[CurrentIndex].nItemCount = item.second;
				NextIndex++;
			}
		}
		else // 다른 아이템 일때
		{
			CurrentIndex = NextIndex;
			m_arrInventory[CurrentIndex].nItemCount = item.second; // 아이템 개수 설정
			m_arrInventory[CurrentIndex].nItemID = item.first; // 아이템 ID 설정
			NextIndex++;
		}
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
	Swap<FS_InventorySlotData>(*pSrcSlotData,*pDstSlotData);
}

bool UC_InventoryComponent::pushItem(int nItemID, int nItemCount)
{
	FS_InventorySlotData* pSlotData = &m_sDummyItemData;
	bool bStackable = m_pItemDataSubsystem->hasItemStateFlag(nItemID, (int32)E_EItemState::CanStackable);
	for (int i = 0; i < m_nInventorySize && pSlotData  == &m_sDummyItemData; i++)
	{
		if (m_arrInventory[i].nItemID == m_pItemDataSubsystem->getUnValidItemID())
			//|| (nItemID == m_arrInventory[i].nItemID && bStackable))
		{
			pSlotData = &m_arrInventory[i];
		}
	}
	if (pSlotData != &m_sDummyItemData)
	{
		pSlotData->nItemID = nItemID;
		pSlotData->nItemCount += nItemCount;
		if (m_onPushItem.IsBound())
			m_onPushItem.Broadcast(pSlotData->nItemID, pSlotData->nItemCount);
	}
	else
	{
		FS_GameAlertSubsystemConfig config{};
		config.strDefaultAlertMessage = FText::FromString(TEXT("인벤토리의 공간이 없습니다."));
		UC_GameAlertSubsystem::pushAlertMessage_Cpp(config);
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

int UC_InventoryComponent::getItemCount(int nY, int nX) 
{
	FS_InventorySlotData* pSlotData = getInventorySlotData(nY, nX);
	if (pSlotData == &m_sDummyItemData)
		return 0;
	return pSlotData->nItemCount;
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
	pItemSlot->nItemID = m_pItemDataSubsystem->getUnValidItemID(); // 아이템 ID를 -1로 설정하여 해당 슬롯을 비움
	pItemSlot->nItemCount = 0; // 아이템 개수도 0으로 설정
	pItemSlot->bLockSort = false;
}


