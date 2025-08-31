#include "C_ItemActorBase.h"
#include "C_ItemDataSubsystem.h"

AC_ItemActorBase::AC_ItemActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
	SetLifeSpan(10.0f);
}

bool AC_ItemActorBase::useItemActor()
{
	bool bResult = findActor(m_pTargetActor) && checkItemUseable();
	if (bResult)
	{
		bResult = itemEffect();
	}
	if (bResult)
		useSuccess();
	else
		useFail();
	m_pTargetActor = nullptr;
	return bResult;
}

bool AC_ItemActorBase::findActor_Implementation(AActor*& pTargetActor)
{
	return true;
}

bool AC_ItemActorBase::itemEffect_Implementation()
{
	return true;
}

bool AC_ItemActorBase::useFail_Implementation()
{
	return false;
}

bool AC_ItemActorBase::checkItemUseable_Implementation()
{
	return true;
}

bool AC_ItemActorBase::useSuccess_Implementation()
{
	return false;
}

FText AC_ItemActorBase::getItemName_Implementation() const
{
	FS_ItemData Data{};
	if(!UC_ItemDataSubsystem::getItemDataByID_CPP(m_nItemID, Data))
		return FText();
	return Data.strItemName;
}

FText AC_ItemActorBase::getItemDesc_Implementation() const
{
	FS_ItemData Data{};
	if (!UC_ItemDataSubsystem::getItemDataByID_CPP(m_nItemID, Data))
		return FText();
	return FText::FromString( Data.strItemDescription );
}
