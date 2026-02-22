#include "C_InventoryComponent.h"
#include <C_GameAlertSubsystem.h>
#include "C_ItemActorBase.h"
#include "GamePlay/C_DataMangerSubsystem.h"


DEFINE_LOG_CATEGORY_STATIC(C_InventoryComponent, Log, All);

UC_InventoryComponent::UC_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	if (m_nInventorySize <= 0)
	{
		m_nInventoryHeight = 10;
		m_nInventoryWidth = 10;
		m_nInventorySize = m_nInventoryHeight * m_nInventoryWidth;
		m_arrInventory.Init(FS_InventorySlot{}, m_nInventorySize);
	}
}

int UC_InventoryComponent::getItemID(int nY, int nX) 
{
	FS_InventorySlot* pSlotData = getInventorySlotData(nY, nX);
	return pSlotData->sData.nItemID;
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
		if (m_pItemDataSubsystem->isValidItemID(m_arrInventory[i].sData.nItemID) && !m_arrInventory[i].sData.bLockSort)
		{
			ItemQueue.push({ m_arrInventory[i].sData.nItemID, m_arrInventory[i].sData.nItemCount });
			resetItemSlot(&m_arrInventory[i]);
		}
	}

	int NextIndex = 0; // 다음 아이템이 삽입될 인덱스
	int CurrentIndex = 0; // 현재 아이템이 삽입 되는 인덱스
	while (!ItemQueue.empty() && CurrentIndex < m_nInventorySize)
	{
		// 다음 인덱스가 유효한지 확인하고, 아이템이 유효하지 않거나 잠금 상태가 아닌 경우에만 진행
		while (NextIndex < m_nInventorySize && 
			m_arrInventory[NextIndex].sData.nItemID != m_pItemDataSubsystem->getUnValidItemID() &&
			m_arrInventory[NextIndex].sData.bLockSort)
		{
			NextIndex++;
		}
		pair<int, int> item = ItemQueue.top();
		ItemQueue.pop();
		
		// Check if the current item can be stacked with the existing item at CurrentIndex
		if (item.first == m_arrInventory[CurrentIndex].sData.nItemID)
		{
			// 같은 아이템 이고 스택 가능 할때
			if (m_pItemDataSubsystem->hasItemStateFlag(item.first, (int32)E_EItemState::CanStackable))
			{
				m_arrInventory[CurrentIndex].sData.nItemCount += item.second;
			}
			else // 같은 아이템 이지만 스택 불가능 할때
			{
				CurrentIndex = NextIndex;
				m_arrInventory[CurrentIndex].sData.nItemID = item.first;
				m_arrInventory[CurrentIndex].sData.nItemCount = item.second;
				NextIndex++;
			}
		}
		else // 다른 아이템 일때
		{
			CurrentIndex = NextIndex;
			m_arrInventory[CurrentIndex].sData.nItemCount = item.second; // 아이템 개수 설정
			m_arrInventory[CurrentIndex].sData.nItemID = item.first; // 아이템 ID 설정
			NextIndex++;
		}
		runSlotChangeInterface(&m_arrInventory[CurrentIndex]);
	}
}

void UC_InventoryComponent::setItemID(int nY, int nX, int nVal)
{
	FS_InventorySlot* pSlotData = getInventorySlotData(nY, nX);
	if (pSlotData == &m_sDummyItemData)
		return;
	pSlotData->sData.nItemID = nVal;
	runSlotChangeInterface(pSlotData);
}

void UC_InventoryComponent::getInventorySlotData(int nY, int nX, FS_InventorySlotData& sData)
{
	FS_InventorySlot* pSlotData = getInventorySlotData(nY, nX);
	if (pSlotData == &m_sDummyItemData)
		return;
	sData = pSlotData->sData;
}

void UC_InventoryComponent::swapInventorySlot(int nSrcY, int nSrcX, int nDstY, int nDstX)
{
	FS_InventorySlot* pSrcSlotData = getInventorySlotData(nSrcY, nSrcX);
	FS_InventorySlot* pDstSlotData = getInventorySlotData(nDstY, nDstX);
	if (pSrcSlotData == &m_sDummyItemData || pDstSlotData == &m_sDummyItemData)
		return;
	Swap<FS_InventorySlotData>(pSrcSlotData->sData,pDstSlotData->sData);
	runSlotChangeInterface(pSrcSlotData);
	runSlotChangeInterface(pDstSlotData);
}


/**
* 아이템을 인벤토리에 추가합니다.
* @param nItemID - Item Key Value
* @param nItemCount - Item Count
*/
bool UC_InventoryComponent::pushItem(int nItemID, int nItemCount)
{
	FS_InventorySlot* pSlotData = &m_sDummyItemData;
	bool bStackable = m_pItemDataSubsystem->hasItemStateFlag(nItemID, (int32)E_EItemState::CanStackable);
	// 1. 스택 가능한 아이템이면 첫번째로 발견되는 같은 아이템 슬롯 선택
	// 2. 스택 불가능한 아이템이거나 같은 아이템이 없으면 빈 슬롯 선택
	// 3. 빈 슬롯이 없으면 실패
	for (int i = 0; i < m_nInventorySize && pSlotData  == &m_sDummyItemData; i++)
	{
		if (m_arrInventory[i].sData.nItemID == m_pItemDataSubsystem->getUnValidItemID())
			//|| (nItemID == m_arrInventory[i].sData.nItemID && bStackable))
		{
			pSlotData = &m_arrInventory[i];
		}
	}

	// 4. 슬롯이 있으면 아이템 추가
	if (pSlotData != &m_sDummyItemData)
	{
		pSlotData->sData.nItemID = nItemID;
		pSlotData->sData.nItemCount += nItemCount;
		int& sCount = m_mapItemCount.FindOrAdd(pSlotData->sData.nItemID);
		sCount += pSlotData->sData.nItemCount;
		if (m_onPushItem.IsBound())
		{
			m_onPushItem.Broadcast(pSlotData->sData.nItemID, pSlotData->sData.nItemCount);
		}
		runSlotChangeInterface(pSlotData);
	}
	else
	{
		// 화면에 알림 띄우기 인벤토리 공간 없음
		FS_GameAlertSubsystemConfig config{};
		config.strDefaultAlertMessage = FText::FromString(TEXT("인벤토리의 공간이 없습니다."));
		UC_GameAlertSubsystem::pushAlertMessage_Cpp(config);
	}
	return  pSlotData != &m_sDummyItemData;
}

void UC_InventoryComponent::setItemSlotLock(int nY, int nX, bool bLock)
{
	FS_InventorySlot* pSlotData = getInventorySlotData(nY, nX);
	if (pSlotData == &m_sDummyItemData)
		return;
	pSlotData->sData.bLockSort  = bLock;
	runSlotChangeInterface(pSlotData);
}

bool UC_InventoryComponent::getItemSlotlock(int nY, int nX)
{
	FS_InventorySlot* pSlotData = getInventorySlotData(nY, nX);
	if (pSlotData == &m_sDummyItemData)
		return false;
	return pSlotData->sData.bLockSort;
}

bool UC_InventoryComponent::getItemCountAtSlot(int nY, int nX, int& nCount)
{
	FS_InventorySlot* pSlotData = getInventorySlotData(nY, nX);
	if (pSlotData == &m_sDummyItemData)
		return false;
	nCount = pSlotData->sData.nItemCount;
	return true;
}

bool UC_InventoryComponent::getItemCountByID(int nItemID, int& nCount)
{
	nCount = 0;
	int* pCount = m_mapItemCount.Find(nItemID);
	if (!pCount)
		return false;
	nCount = *pCount;
	return true;
}

/**
* 아이템을 인벤토리에서 제거합니다.
* @param nItemID - Item Key Value
* @param nItemCount - Item Count
*/
bool UC_InventoryComponent::removeItem(int nItemID, int nCount)
{
	TArray<FS_InventorySlot*> arrSlotData{};
	arrSlotData.Reserve(m_nInventorySize);
	int nSum = nCount;
	for (int i = 0; i < m_nInventorySize && nSum > 0; i++)
	{
		if (m_arrInventory[i].sData.nItemID == nItemID && nSum > 0)
		{
			arrSlotData.Push(&m_arrInventory[i]);
			nSum -= m_arrInventory[i].sData.nItemCount;
		}
	}

	if (nSum > 0)
	{
		// 화면에 알림 띄우기
		FS_GameAlertSubsystemConfig config{};
		config.strDefaultAlertMessage = FText::FromString(TEXT("아이템이 충분하지 않습니다."));
		if (nItemID == UC_ItemDataSubsystem::getUnValidItemID_CPP())
			config.strDefaultAlertMessage = FText::FromString(TEXT(""));
		UC_GameAlertSubsystem::pushAlertMessage_Cpp(config);
		return false;
	}

	int& sCount = m_mapItemCount.FindOrAdd(nItemID);
	sCount -= nCount;
	if (sCount <= 0)
		m_mapItemCount.Remove(nItemID);

	for (FS_InventorySlot* pSlotData : arrSlotData)
	{
		if (pSlotData->sData.nItemCount > nCount)
		{
			pSlotData->sData.nItemCount -= nCount;
			nCount = 0;
		}
		else
		{
			nCount -= pSlotData->sData.nItemCount;
			resetItemSlot(pSlotData);
		}
		runSlotChangeInterface(pSlotData);
	}
	return true;
}

bool UC_InventoryComponent::removeItemAtSlot(int nY, int nX, int nCount)
{
	FS_InventorySlot* pSlotData = getInventorySlotData(nY, nX);
	if (pSlotData == &m_sDummyItemData || pSlotData->sData.nItemID == m_pItemDataSubsystem->getUnValidItemID())
		return false;
	if (pSlotData->sData.nItemCount >= nCount)
	{
		int& sCount = m_mapItemCount.FindOrAdd(pSlotData->sData.nItemID);
		sCount -= nCount;
		pSlotData->sData.nItemCount -= nCount;
		if (pSlotData->sData.nItemCount <= 0)
		{
			resetItemSlot(pSlotData);
			m_mapItemCount.Remove(pSlotData->sData.nItemID);
		}
		runSlotChangeInterface(pSlotData);
		return true;
	}
	FS_GameAlertSubsystemConfig config{};
	config.strDefaultAlertMessage = FText::FromString(TEXT("아이템이 충분하지 않습니다."));
	UC_GameAlertSubsystem::pushAlertMessage_Cpp(config);
	return false;
}

void UC_InventoryComponent::setSlotInterface(int nY, int nX, UObject* pInterface)
{
	FS_InventorySlot* pSlotData = getInventorySlotData(nY, nX);
	if (pSlotData == &m_sDummyItemData || !pInterface || !pInterface->Implements<UC_InventorySlotInterface>())
		return ;
	pSlotData->pSlotInterface = TScriptInterface< IC_InventorySlotInterface>(pInterface);

	runSlotChangeInterface(pSlotData);
}

void UC_InventoryComponent::setInventorySlotData(int nY, int nX, FS_InventorySlotData& sData)
{
	FS_InventorySlot* pSlotData = getInventorySlotData(nY, nX);
	if (!pSlotData)
		return;
	pSlotData->sData = sData;
	if (sData.nItemID == m_pItemDataSubsystem->getUnValidItemID())
		return;
	int& sCount = m_mapItemCount.FindOrAdd(sData.nItemID);
	sCount += sData.nItemCount;
}

bool UC_InventoryComponent::useItemAtSlot(int nY, int nX, int nCount)
{
	FS_InventorySlot* pSlotData = getInventorySlotData(nY, nX);
	if (pSlotData == &m_sDummyItemData)
		return false;
	int MaxCount = nCount;
	while (MaxCount > 0)
	{
		AC_ItemActorBase* pItem = m_pItemDataSubsystem->spawnEffectItem(pSlotData->sData.nItemID, Cast<APlayerController>(GetOwner())->AcknowledgedPawn);
		if (pItem && pItem->useItemActor())
		{
			removeItemAtSlot(nY, nX, 1);
		}
		MaxCount--;
	}
	return MaxCount <= 0;
}

void UC_InventoryComponent::BeginPlay()
{
	if (Cast<APlayerController>(GetOwner()))
	{
		UC_DataMangerSubsystem::loadData(this);
	}

	UActorComponent::BeginPlay();

	if (!GetWorld())
		return;
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		m_pItemDataSubsystem = GameInstance->GetSubsystem<UC_ItemDataSubsystem>();
		//m_pItemDataSubsystem->loadInventroyData(this);
	}
}

#if WITH_EDITOR
void UC_InventoryComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property)
	{
		m_nInventorySize = m_nInventoryHeight * m_nInventoryWidth;
		if (m_nInventorySize <= 0)
		{
			m_nInventoryHeight = 1;
			m_nInventoryWidth = 1;
			m_nInventorySize = m_nInventoryHeight * m_nInventoryWidth;
		}
	}
}
#endif

bool UC_InventoryComponent::isBound(int nY, int nX)  const
{
	if (nY < 0 ||  nY >= m_nInventoryHeight || nX < 0 || nX >= m_nInventoryWidth)
		return false;
	return true;
}

FS_InventorySlot* UC_InventoryComponent::getInventorySlotData(int nY, int nX)
{
	if (nY < 0 || nY >= m_nInventoryHeight || nX < 0 || nX >= m_nInventoryWidth || !m_arrInventory.IsValidIndex(getArrayIndex(nY, nX)))
		return &m_sDummyItemData;
	return &m_arrInventory[getArrayIndex(nY, nX)];
}

int UC_InventoryComponent::getArrayIndex(int nY, int nX) const
{ 
	return nY * m_nInventoryWidth + nX;
}

void UC_InventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UActorComponent::EndPlay(EndPlayReason);
	if (Cast<APlayerController>(GetOwner()))
	{
		UC_DataMangerSubsystem::saveBinaryData(this);
	}
}

void UC_InventoryComponent::resetItemSlot(FS_InventorySlot* pItemSlot)
{
	pItemSlot->sData.nItemID = m_pItemDataSubsystem->getUnValidItemID(); // 아이템 ID를 -1로 설정하여 해당 슬롯을 비움
	pItemSlot->sData.nItemCount = 0; // 아이템 개수도 0으로 설정
	pItemSlot->sData.bLockSort = false;
	runSlotChangeInterface(pItemSlot);
}

void UC_InventoryComponent::runSlotChangeInterface(FS_InventorySlot* pItemSlot)
{
	if (pItemSlot->pSlotInterface && pItemSlot->pSlotInterface.GetInterface())
	{
		pItemSlot->pSlotInterface.GetInterface()->slotChangeEvent(&pItemSlot->sData);
	}
}

E_DataType UC_InventoryComponent::getDataType()
{
	return E_DataType::E_Binary;
}

FString UC_InventoryComponent::getFilePath(E_DataType eType)
{
	return  FPaths::ProjectSavedDir() + TEXT("InventoryData");
}

void UC_InventoryComponent::loadBinaryData(TArray<uint8>& arData)
{
	FS_InventorySaveData Data(m_nInventoryWidth, m_nInventoryHeight, m_arrInventory);
	if (!UC_DataMangerSubsystem::readBinaryFile(arData, &Data))
		return;
	m_nInventoryHeight = Data.nInventoryWidth;
	m_nInventoryWidth = Data.nInventoryHeight;
	m_nInventorySize = m_nInventoryHeight * m_nInventoryWidth;
	for (int i = 0; i < m_nInventoryHeight; i++)
	{
		for (int j = 0; j < m_nInventoryWidth; j++)
		{
			setInventorySlotData(i, j, Data.arrInventory[getArrayIndex(i,j)].sData);
		}
	}
}

TArray<uint8> UC_InventoryComponent::getBinaryData()
{
	FS_InventorySaveData Data(m_nInventoryWidth, m_nInventoryHeight, m_arrInventory);
	TArray<uint8> result{};
	// FMemoryWriter에 의해 구조체를 바이트 배열로 변환하는 함수
	UC_DataMangerSubsystem::saveBinaryFile< FS_InventorySaveData>(result, &Data);
	return result;
}

FS_InventorySaveData::FS_InventorySaveData(int Width, int Height, TArray<FS_InventorySlot>& Inventory)
	: arrInventory{}, nInventoryWidth{}, nInventoryHeight{}
{
	nInventoryWidth = Width;
	nInventoryHeight = Height;
	arrInventory = Inventory;
}
