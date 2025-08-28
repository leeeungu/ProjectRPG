// Fill out your copyright notice in the Description page of Project Settings.


#include "C_StaggerComponent.h"



// Sets default values for this component's properties
UC_StaggerComponent::UC_StaggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UC_StaggerComponent::applyStagger(float fStagger)
{
	if (m_bIsBroken)
		return;

	m_fCurrentStagger += fStagger;

	if (m_fCurrentStagger >= m_fMaxStagger)
	{
		m_bIsBroken = true;
		m_fBreakTimer = m_fBrokenDuration;

		m_onBroken.Broadcast();
	}
}

bool UC_StaggerComponent::isBroken()
{
	return m_bIsBroken;
}

void UC_StaggerComponent::setStaggerPoint(float fStagger)
{
	m_fMaxStagger = fStagger;
}

void UC_StaggerComponent::setBreakDuration(float fDuration)
{
	m_fBrokenDuration = fDuration;
}

float UC_StaggerComponent::getCurrentStaggerPoint() const
{
	return m_fCurrentStagger;
}

void UC_StaggerComponent::recover()
{
	m_bIsBroken = false;
	m_fCurrentStagger = 0.f;
	m_onRecover.Broadcast();
}


// Called when the game starts
void UC_StaggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UC_StaggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_bIsBroken)
	{
		m_fBreakTimer -= DeltaTime;
		if (m_fBreakTimer <= 0)
		{
			recover();
		}
	}
	
}

