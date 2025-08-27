// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/C_StaggerGimmickComponent.h"

DEFINE_LOG_CATEGORY_STATIC(C_StaggerGimmickComponent, Log, All);

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

UC_StaggerGimmickComponent::UC_StaggerGimmickComponent()
{
	m_fTriggerHp = 40.f;
	m_fGimmickTime = 150.f;
}
