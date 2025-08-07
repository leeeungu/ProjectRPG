#include "C_ItemActorBase.h"

AC_ItemActorBase::AC_ItemActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
	SetLifeSpan(10.0f);
}

bool AC_ItemActorBase::useItemActor()
{
	//m_pInstigator = pInstigator;
	if (findActor(m_pTargetActor) && itemEffect())
	{
		return true;
	}
	useFail();
	m_pInstigator = nullptr;
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

void AC_ItemActorBase::BeginPlay()
{
	Super::BeginPlay();
}