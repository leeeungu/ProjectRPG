#include "C_ItemDataSubsystem.h"
#include "Kismet/DataTableFunctionLibrary.h" 

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
    if (m_pItemDataTable)
    {
        TArray< FS_ItemData*>  arRow{};
        m_pItemDataTable->GetAllRows("", arRow);
        for (FS_ItemData* Row : arRow)
        {
            m_mapItemData.FindOrAdd(Row->nItemID, Row);
        }
    }
}

void UC_ItemDataSubsystem::Deinitialize()
{
    m_mapItemData.Empty();
	Super::Deinitialize();
}

bool UC_ItemDataSubsystem::getItemDataByID(int ItemID, FS_ItemData& OutData) const
{
    FS_ItemData* pItemData = getItemDataByID_Internal(ItemID);
    if (pItemData)
        OutData = *pItemData;
    return pItemData != nullptr;
}

bool UC_ItemDataSubsystem::isValidItemID(int ItemID) const
{
    return ItemID != getUnValidItemID();
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
        return (pItemData->eltemState & Bitmask) != 0;
	}
    return false;
}