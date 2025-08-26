#include "Item/Actor/C_EquipItem.h"
#include "C_ItemDataSubsystem.h"
#include "Item/Component/C_EquipComponent.h"

AC_EquipItem::AC_EquipItem() : AC_ItemActorBase{}
{
}

FText AC_EquipItem::getItemDesc_Implementation() const
{
	FS_ItemData Data{};
	if (!UC_ItemDataSubsystem::getItemDataByID_CPP(m_nItemID, Data))
		return FText();
	FTextFormat format{};
	format.FromString(Data.strItemDescription);
	return FText::Format<float>(format, m_fEquipValue);
}

bool AC_EquipItem::findActor_Implementation(AActor*& pTargetActor)
{
	pTargetActor = GetInstigator()->GetInstigatorController();
	return pTargetActor != nullptr;
}

bool AC_EquipItem::itemEffect_Implementation()
{
	m_pTargetActor = GetInstigator()->GetInstigatorController();
	if (!m_pTargetActor)
		return false;
	UC_EquipComponent* pComponent = m_pTargetActor->GetComponentByClass<UC_EquipComponent>();
	if (pComponent)
	{
		if (m_pEquipItemDataTable)
		{
			TArray< FS_EquipItemData*> arrData{};
			m_pEquipItemDataTable->GetAllRows(TEXT(""), arrData);
			if (arrData.IsValidIndex(m_nEquipLevel))
			{
				m_fEquipValue = arrData[m_nEquipLevel]->fValue;
			}
		}
		pComponent->registerEquip(this);
		return true;
	}
	return false;
}

bool AC_EquipItem::useFail_Implementation()
{
	Destroy();
	return true;
}

bool AC_EquipItem::checkItemUseable_Implementation()
{
	if (!m_pTargetActor || !m_pTargetActor->GetComponentByClass<UC_EquipComponent>())
	{
		return false;
	}
	return true;
}

bool AC_EquipItem::useSuccess_Implementation()
{
	return true;
}

