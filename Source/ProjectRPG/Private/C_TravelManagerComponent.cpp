#include "C_TravelManagerComponent.h"

UC_TravelManagerComponent::UC_TravelManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UC_TravelManagerComponent::setTravelType(E_TrabelType eType)
{
	if (m_onTravelChanged.IsBound())
	{
		m_onTravelChanged.Broadcast(m_eTravelType, eType);
	}
	m_eTravelType = eType;
}

void UC_TravelManagerComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
}