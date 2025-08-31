// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/C_StaggerGimmickComponent.h"

DEFINE_LOG_CATEGORY_STATIC(C_StaggerGimmickComponent, Log, All);

float UC_StaggerGimmickComponent::getGoalStagger() const
{
	return m_fGoalStagger;
}

float UC_StaggerGimmickComponent::getBrokenDuration() const
{
	return m_fBrokenDuration;
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
}

void UC_StaggerGimmickComponent::endGimmick()
{
	Super::endGimmick();


	m_onStaggerGimmickEnd.Broadcast();


}

UC_StaggerGimmickComponent::UC_StaggerGimmickComponent()
{
	m_fTriggerHp = 40.f;
	m_fGimmickTime = 150.f;
}
