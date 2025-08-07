#include "C_ItemActorBase.h"

AC_ItemActorBase::AC_ItemActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
	SetLifeSpan(10.0f);
}

bool AC_ItemActorBase::useItemActor()
{
	if (findActor(m_pTargetActor) && checkItemUseable())
	{
		return itemEffect();
	}
	useFail();
	m_pTargetActor = nullptr;
	return false;
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

void AC_ItemActorBase::BeginPlay()
{
	Super::BeginPlay();
}