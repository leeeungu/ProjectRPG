#include "C_TravelManagerComponent.h"

UC_TravelManagerComponent::UC_TravelManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UC_TravelManagerComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
}