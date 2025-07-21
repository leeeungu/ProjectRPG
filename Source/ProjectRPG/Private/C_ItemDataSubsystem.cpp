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
    m_mapItemData.FindOrAdd(m_sDummyItemData.nItemID, &m_sDummyItemData);
    if (m_pItemDataTable)
    {
        for (auto& Pair : m_pItemDataTable->GetRowMap())
        {
            FName RowName = Pair.Key;
            if (const FS_ItemData* Row = m_pItemDataTable->FindRow<FS_ItemData>(RowName, TEXT("Init ItemData")))
            {
                m_mapItemData.FindOrAdd(Row->nItemID, Row);
            }
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
    if (const FS_ItemData* const* Found = m_mapItemData.Find(ItemID))
    {
        OutData = **Found;
        return true;
    }
    return false;
}
