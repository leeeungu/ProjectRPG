#include "C_ItemActorBase.h"

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
	return false;
}

bool AC_ItemActorBase::itemEffect_Implementation()
{
	return false;
}

bool AC_ItemActorBase::useFail_Implementation()
{
	return false;
}

bool AC_ItemActorBase::checkItemUseable_Implementation()
{
	return false;
}

bool AC_ItemActorBase::useSuccess_Implementation()
{
	return false;
}
