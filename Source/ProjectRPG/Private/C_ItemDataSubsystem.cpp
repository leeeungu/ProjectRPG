#include "C_ItemDataSubsystem.h"
#include "Kismet/DataTableFunctionLibrary.h" 
#include "C_ItemActorBase.h"
#include "C_InventoryComponent.h"
#include "C_QuickSlotManagerComponent.h"

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
}

void UC_ItemDataSubsystem::Deinitialize()
{
    m_mapItemData.Empty();
	Super::Deinitialize();
}

UC_ItemDataSubsystem* UC_ItemDataSubsystem::getInstance(UObject* pWorldContextObject)
{
    if (!pWorldContextObject || !pWorldContextObject->GetWorld() || !pWorldContextObject->GetWorld()->GetGameInstance())
        return nullptr;
    return pWorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UC_ItemDataSubsystem>();
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

bool UC_ItemDataSubsystem::getItemDataByID_CPP(UObject* pWorldContextObject, int ItemID, FS_ItemData& OutData)
{
    if (UC_ItemDataSubsystem* pInstance = getInstance(pWorldContextObject))
    {
        return pInstance->getItemDataByID(ItemID, OutData);
    }
    return false;
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

AC_ItemActorBase* UC_ItemDataSubsystem::spawnEffectItem_Cpp(int ItemID, APawn* pInstigator)
{
    UC_ItemDataSubsystem* pInstance = getInstance(pInstigator);
    FS_ItemData* pItemData = pInstance->getItemDataByID_Internal(ItemID);
    if (!pItemData || !pInstigator || !pItemData->cEffectItemClass.Get())
        return nullptr;
    FTransform transfrom = pInstigator->GetActorTransform();

    AC_ItemActorBase* pItem = pInstance->GetWorld()->SpawnActorDeferred< AC_ItemActorBase>(pItemData->cEffectItemClass, transfrom, pInstigator, pInstigator);
    if (pItem)
    {
        pItem->setItemID(ItemID);
        pItem->SetInstigator(pInstigator);
        pItem->FinishSpawning(transfrom);
    }
    return pItem;
}

FS_ItemData* UC_ItemDataSubsystem::getItemDataByID_Internal(int ItemID) const
{   
    if (const FS_ItemData* const* Found = m_mapItemData.Find(ItemID))
    {
        return const_cast<FS_ItemData*>(*Found);
	}
    return nullptr;
}

bool UC_ItemDataSubsystem::hasItemStateFlag(int ItemID, uint8 Bitmask) const
{   
    FS_ItemData* pItemData = getItemDataByID_Internal(ItemID);
    if (pItemData)
    {
        return (pItemData->eltemStateFlag & Bitmask) != 0;
	}
    return false;
}