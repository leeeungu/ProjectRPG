// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PhaseComponent.h"

// Sets default values for this component's properties
UC_PhaseComponent::UC_PhaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UC_PhaseComponent::phaseChange(float fHp)
{
	if (fHp <= 0)
		return;

	float fCurrentHp = fHp;
	int32 nIndex = 0;

	if (fCurrentHp <= m_fChangePercentHp && nIndex != m_nCurrentPhaseIndex)
	{
		m_nCurrentPhaseIndex = nIndex;
		m_onPhaseChange.Broadcast();
	}
	else
		return;
	
}


// Called when the game starts
void UC_PhaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UC_PhaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

