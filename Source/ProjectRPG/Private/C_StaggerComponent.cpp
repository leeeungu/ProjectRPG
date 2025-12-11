// Fill out your copyright notice in the Description page of Project Settings.


#include "C_StaggerComponent.h"
#include "C_CounterComponent.h"
#include "C_MonsterBaseCharacter.h"



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

	switch (getStaggerMode())
	{
	case E_StaggerMode::Normal:
		if (m_pMonster->getIsBlockStagger() == false)
		{
			m_fCurrentStagger += fStagger;

			if (m_fCurrentStagger >= m_fMaxStagger)
			{
				m_bIsBroken = true;
				m_fBreakTimer = m_fBrokenDuration;

				m_onBroken.Broadcast();
			}
		}
		break;

	case E_StaggerMode::Gimmick:
		m_fGimmickCurrentStagger += fStagger;

		if (m_fGimmickCurrentStagger >= m_fGimmickMaxStagger)
		{
			m_bIsBroken = true;
			m_fBreakTimer = m_fGimmickBreakDuration;

			m_onGimmickBroken.Broadcast();
		}
		break;

	default:
		break;
	}
	
}

bool UC_StaggerComponent::isBroken()
{
	return m_bIsBroken;
}

void UC_StaggerComponent::setGimmickMaxStaggerPoint(float fStagger)
{
	m_fGimmickMaxStagger = fStagger;
}

void UC_StaggerComponent::setGimmickStaggerPoint(float fStagger)
{
	m_fGimmickCurrentStagger = fStagger;
}

void UC_StaggerComponent::setGimmickBreakDuration(float fDuration)
{
	m_fGimmickBreakDuration = fDuration;
}

float UC_StaggerComponent::getGimmickMaxStaggerPoint() const
{
	return m_fGimmickMaxStagger;
}

float UC_StaggerComponent::getGimmickCurrentStaggerPoint() const
{
	return m_fGimmickCurrentStagger;
}

void UC_StaggerComponent::setMaxStaggerPoint(float fStagger)
{
	m_fMaxStagger = fStagger;
}

void UC_StaggerComponent::setStaggerPoint(float fStagger)
{
	m_fCurrentStagger = fStagger;
}

void UC_StaggerComponent::setBreakDuration(float fDuration)
{
	m_fBrokenDuration = fDuration;
}

float UC_StaggerComponent::getMaxStaggerPoint() const
{
	return m_fMaxStagger;
}

float UC_StaggerComponent::getCurrentStaggerPoint() const
{
	return m_fCurrentStagger;
}

float UC_StaggerComponent::getCurrentBreakPoint() const
{
	return m_fBrokenDuration;
}

void UC_StaggerComponent::setMode(E_StaggerMode eMode)
{
	if (m_eCurrentMode != eMode)
	{
		m_eCurrentMode = eMode;
		m_onStaggerModeChange.Broadcast(eMode);
	}

	
}

E_StaggerMode UC_StaggerComponent::getStaggerMode() const
{
	return m_eCurrentMode;
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
	m_pCounterCom = GetOwner()->GetComponentByClass<UC_CounterComponent>();

	m_pMonster = Cast<AC_MonsterBaseCharacter>(GetOwner());

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

