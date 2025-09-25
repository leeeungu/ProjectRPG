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
	
	pStaggerCom->setGimmickMaxStaggerPoint(m_fGoalStagger);
	pStaggerCom->setGimmickBreakDuration(m_fBrokenDuration);
	pStaggerCom->setMode(E_StaggerMode::Gimmick);
}

void UC_StaggerGimmickComponent::restoreStagger(UC_StaggerComponent* pStaggerCom)
{
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
	m_fGimmickTime = 30.f;
}
