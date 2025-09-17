// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/C_StaggerGimmickComponent.h"
#include "C_StaggerComponent.h"
#include "C_MonsterBaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(C_StaggerGimmickComponent, Log, All);

float UC_StaggerGimmickComponent::getGoalStagger() const
{
	return m_fGoalStagger;
}

float UC_StaggerGimmickComponent::getBrokenDuration() const
{
	return m_fBrokenDuration;
}

void UC_StaggerGimmickComponent::applyGimmickStagger(UC_StaggerComponent* pStaggerCom)
{

	if (!pStaggerCom)
		return;

	

	if (m_fSavedMax <= 0.f)
	{
		m_fSavedMax = pStaggerCom->getMaxStaggerPoint();
		m_fSavedBreak = pStaggerCom->getCurrentBreakPoint();
		m_fSavedCurStagger = pStaggerCom->getCurrentStaggerPoint();

		UE_LOG(LogTemp, Warning, TEXT("ApplyGimmickStagger: Max=%f, Cur=%f"),
			pStaggerCom->getMaxStaggerPoint(),
			pStaggerCom->getCurrentStaggerPoint());
	}

	

	pStaggerCom->setMaxStaggerPoint(m_fGoalStagger);
	pStaggerCom->setBreakDuration(m_fBrokenDuration);
	pStaggerCom->setMode(E_StaggerMode::Gimick);
}

void UC_StaggerGimmickComponent::restoreStagger(UC_StaggerComponent* pStaggerCom)
{
	UE_LOG(LogTemp, Warning, TEXT("Restore - SavedMax: %f, SavedCur: %f"),
		m_fSavedMax, m_fSavedCurStagger);
	if (!pStaggerCom)
		return;
	if (m_fSavedMax > 0.f)
	{
		pStaggerCom->setMaxStaggerPoint(m_fSavedMax);
		pStaggerCom->setBreakDuration(m_fSavedBreak);
		pStaggerCom->setStaggerPoint(m_fSavedCurStagger);

		UE_LOG(LogTemp, Warning, TEXT("ApplyGimmickStagger: Max=%f, Cur=%f"),
			pStaggerCom->getMaxStaggerPoint(),
			pStaggerCom->getCurrentStaggerPoint());
	}
	

	m_fSavedMax = 0.f;
	m_fSavedBreak = 0.f;
	m_fSavedCurStagger = 0.f;
	pStaggerCom->setMode(E_StaggerMode::Normal);
}

void UC_StaggerGimmickComponent::BeginPlay()
{
	Super::BeginPlay();

	if (m_pMonster)
	{
		m_pStaggerCom = m_pMonster->FindComponentByClass<UC_StaggerComponent>();
		if (!m_pStaggerCom)
		{
			UE_LOG(LogTemp, Warning, TEXT("StaggerComponent not found!"));
		}
	}
}

bool UC_StaggerGimmickComponent::canGimmickStart(float fHp, float fMaxHp)
{
	if (!Super::canGimmickStart(fHp, fMaxHp))
		return false;


	return true;
}

void UC_StaggerGimmickComponent::excuteGimmick()
{
	Super::excuteGimmick();

	UE_LOG(C_StaggerGimmickComponent, Error, TEXT("excute Gimmick!!!!!"));

	applyGimmickStagger(m_pStaggerCom);


}

void UC_StaggerGimmickComponent::endGimmick()
{
	Super::endGimmick();


	m_onStaggerGimmickEnd.Broadcast();


}

UC_StaggerGimmickComponent::UC_StaggerGimmickComponent()
{
	m_fTriggerHp = 40.f;
	m_fGimmickTime = 200.f;
}
