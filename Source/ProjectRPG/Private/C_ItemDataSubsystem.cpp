#include "C_ItemDataSubsystem.h"
#include "Kismet/DataTableFunctionLibrary.h" 
#include "C_ItemActorBase.h"
#include "C_InventoryComponent.h"
#include "C_QuickSlotManagerComponent.h"

UC_ItemDataSubsystem* UC_ItemDataSubsystem::m_pInstance = nullptr;

UC_ItemDataSubsystem::UC_ItemDataSubsystem()  
{
    //Script/Engine.DataTable'/Game/Item/DataTable/DT_ItemData.DT_ItemData'
    static ConstructorHelpers::FObjectFinder<UDataTable> pObject(*m_strDataTablePath);
    if (pObject.Succeeded())
        m_pItemDataTable = pObject.Object;
}

void UC_ItemDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (!m_pItemDataTable)
        m_pItemDataTable = LoadObject<UDataTable>(NULL, *m_strDataTablePath, NULL, LOAD_None, NULL);

    for (int i = (uint8)E_QuickSlotType::E_None; i < (uint8)E_QuickSlotType::E_QuickSlot_MAX; i++)
    {
        m_arrQuickSlotItem[i] = getUnValidItemID_CPP();
    }

    if (m_pItemDataTable)
    {
        TArray< FS_ItemData*>  arRow{};
        m_pItemDataTable->GetAllRows("", arRow);
        for (FS_ItemData* Row : arRow)
        {
            const FS_ItemData*& pData = m_mapItemData.FindOrAdd(Row->nItemID, nullptr);
            if (!pData)
            {
                pData = Row;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Item ID %d already exists in the map!"), Row->nItemID);
			}
        }
    }
    if (!m_pInstance)
        m_pInstance = this;
}

void UC_ItemDataSubsystem::Deinitialize()
{
    if (m_pInstance)
        m_pInstance = nullptr;
    m_mapItemData.Empty();
	Super::Deinitialize();
}

bool UC_ItemDataSubsystem::getItemDataByID(int ItemID, FS_ItemData& OutData) const
{
    OutData = FS_ItemData{};
    FS_ItemData* pItemData = getItemDataByID_Internal(ItemID);
    if (pItemData && pItemData->nItemID == getUnValidItemID())
        pItemData = nullptr;
    if (pItemData)
        OutData = *pItemData;
    return pItemData != nullptr;
}

bool UC_ItemDataSubsystem::getItemDataByID_CPP(int ItemID, FS_ItemData& OutData) 
{
    return m_pInstance->getItemDataByID(ItemID, OutData);
}

bool UC_ItemDataSubsystem::isValidItemID(int ItemID) const
{
    return ItemID != getUnValidItemID();
}

AC_ItemActorBase* UC_ItemDataSubsystem::spawnEffectItem(int ItemID, APawn* pInstigator)
{
    FS_ItemData* pItemData = getItemDataByID_Internal(ItemID);
    if (!pItemData || !pInstigator || !pItemData->cEffectItemClass.Get())
        return nullptr;
    FTransform transfrom = pInstigator->GetActorTransform();

    AC_ItemActorBase* pItem = GetWorld()->SpawnActorDeferred< AC_ItemActorBase>(pItemData->cEffectItemClass, transfrom, pInstigator, pInstigator);
    if (pItem)
    {
		pItem->setItemID(ItemID);
        pItem->SetInstigator(pInstigator);
        pItem->FinishSpawning(transfrom);
    }
    return pItem;
}

void UC_ItemDataSubsystem::loadInventroyData(UC_InventoryComponent* pInventory)
{
    if (!pInventory || !Cast<APlayerController>(pInventory->GetOwner()))
        return;

    int nWidth = pInventory->getInventoryWidth();
    int nHeight = pInventory->getInventoryHeight();
    if (m_arrInventory.Num() != nWidth * nHeight)
        return;
    for (int i = 0; i < nHeight; i++)
    {
        for (int j = 0; j < nWidth; j++)
        {
            pInventory->setInventorySlotData(i, j, m_arrInventory[pInventory->getArrayIndex(i, j)]);
        }
    }
}

void UC_ItemDataSubsystem::saveInventroyData(UC_InventoryComponent* pInventory)
{
    if (!pInventory || !Cast<APlayerController>(pInventory->GetOwner()))
        return;

    m_arrInventory.Empty();
	int nWidth = pInventory->getInventoryWidth();
	int nHeight = pInventory->getInventoryHeight();
	m_arrInventory.Init(FS_InventorySlotData{}, pInventory->getInventorySize());
    for (int i = 0; i < nHeight; i++)
    {
        for (int j = 0; j < nWidth; j++)
        {
            pInventory->getInventorySlotData(i, j, m_arrInventory[pInventory->getArrayIndex(i,j)]);
        }
    }
}

void UC_ItemDataSubsystem::loadQuickSlotData(UC_QuickSlotManagerComponent* pQuickSlot)
{
    if (!pQuickSlot)
        return;

    for (int i = (uint8)E_QuickSlotType::E_None + 1; i < (uint8)E_QuickSlotType::E_QuickSlot_MAX; i++)
    {
        pQuickSlot->setQuickSlotItem((E_QuickSlotType)i, m_arrQuickSlotItem[i]);
    }
}

void UC_ItemDataSubsystem::saveQuickSlotData(UC_QuickSlotManagerComponent* pQuickSlot)
{
    if (!pQuickSlot)
        return;

    for (int i = (uint8)E_QuickSlotType::E_None + 1; i < (uint8)E_QuickSlotType::E_QuickSlot_MAX; i++)
    {
        m_arrQuickSlotItem[i] = pQuickSlot->getQuickSlotID((E_QuickSlotType)i);
    }
}

FS_ItemData* UC_ItemDataSubsystem::getItemDataByID_Internal(int ItemID) const
{   
    if (const FS_ItemData* const* Found = m_mapItemData.Find(ItemID))
    {
        return const_cast<FS_ItemData*>(*Found);
	}
    return nullptr;
}

bool UC_ItemDataSubsystem::hasItemStateFlag(int ItemID,  int32 Bitmask) const
{   
    FS_ItemData* pItemData = getItemDataByID_Internal(ItemID);
    if (pItemData)
    {
        return (pItemData->eltemStateFlag & Bitmask) != 0;
	}
    return false;
}