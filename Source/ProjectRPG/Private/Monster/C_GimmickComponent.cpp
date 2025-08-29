// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/C_GimmickComponent.h"


DEFINE_LOG_CATEGORY_STATIC(C_GimmickGimmickComponent, Log, All);

bool UC_GimmickComponent::canGimmickStart(float fHp, float fMaxHp)
{
	if (m_bGimmickPlaying)
		return false;

	float fHpRatio = (fHp / fMaxHp) * 100.f;
	
	if (fHpRatio <= m_fTriggerHp)
	{
		UE_LOG(C_GimmickGimmickComponent, Error, TEXT("HP: [%s] %.1f / %.1f , Trigger: %.1f"),*this->GetName(), fHp, fMaxHp, m_fTriggerHp);
		return true;
	}
		
	/*
	* 현재 기믹이 실행중이면 false 반환
	* 몬스터의 hp 비율이 트리거hp보다 작거나 같으면 true를 반환
	*/


	return false;
}

// Sets default values for this component's properties
UC_GimmickComponent::UC_GimmickComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UC_GimmickComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UC_GimmickComponent::excuteGimmick()
{
	m_bGimmickPlaying = true;


}

float UC_GimmickComponent::getGimmickTime() const
{
	return m_fGimmickTime;
}

bool UC_GimmickComponent::IsPlayingGimmick()
{
	return m_bGimmickPlaying;
}

void UC_GimmickComponent::endGimmick()
{
	m_bGimmickPlaying = false;

}


// Called every frame
void UC_GimmickComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_bGimmickPlaying)
		m_fGimmickTime -= DeltaTime;

	if (m_fGimmickTime <= 0)
		endGimmick();

	// ...
}

