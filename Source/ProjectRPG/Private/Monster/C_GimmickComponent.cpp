// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/C_GimmickComponent.h"
#include "C_MonsterBaseCharacter.h"
#include "../Public/Monster/C_GimmickStartPos.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(C_GimmickGimmickComponent, Log, All);

// Called when the game starts
void UC_GimmickComponent::BeginPlay()
{
	Super::BeginPlay();


	// ...

	m_pMonster = Cast<AC_MonsterBaseCharacter>(GetOwner());

	if (m_pMonster)
	{
		m_pAnim = m_pMonster->GetMesh()->GetAnimInstance();
	}

	TArray<AActor*> arrFound{};

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_GimmickStartPos::StaticClass(), arrFound);

	if (arrFound.Num() > 0)
	{
		AActor* pFoundActor = arrFound[0];
		FVector vFoundPos = pFoundActor->GetActorLocation();
		m_vGimmickPos = vFoundPos;
	}

}

bool UC_GimmickComponent::canGimmickStart(float fHp, float fMaxHp)
{
	if (m_bGimmickPlaying)
		return false;

	float fHpRatio = (fHp / fMaxHp) * 100.f;
	
	if (fHpRatio <= m_fTriggerHp)
	{
		if (!canPlayGimmickMontage())
		{
			m_pMonster->reserveGimmick(m_pMonster->getHp(), m_pMonster->getMaxHp());
			return false;
		}
		return true;
	}
		
	/*
	* 현재 기믹이 실행중이면 false 반환
	* 몬스터의 hp 비율이 트리거hp보다 작거나 같으면 true를 반환
	*/


	return false;
}

void UC_GimmickComponent::startGimmick()
{

	if (!m_pMonster || !m_pAnim)
		return;

	m_pMonster->setActivePower(true);


	m_pMonster->setPhaseState(E_MonsterPhaseState::GimmickReady);

	m_pMonster->stopAi();

	

	if (m_pGimmickStartMontage)
	{
		playMontageWithCallBack(m_pGimmickStartMontage, TEXT("onStartGimmickMontageEnded"));
	}
	else
	{
		onStartGimmickMontageEnded(nullptr, false);
	}

}

void UC_GimmickComponent::onStartGimmickMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!m_pMonster)
		return;

	if (m_pAnim)
	{
		m_pAnim->OnMontageEnded.RemoveAll(this);
	}

	m_pMonster->TeleportTo(m_vGimmickPos, FRotator::ZeroRotator);


	// 기믹 실행으로 넘어감
	excuteGimmick();
}

// Sets default values for this component's properties
UC_GimmickComponent::UC_GimmickComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UC_GimmickComponent::playMontageWithCallBack(UAnimMontage* pMontage, FName strFunctionName)
{
	if (!m_pAnim || !pMontage)
		return;

	if (m_pAnim->IsAnyMontagePlaying())
	{
		// 몽타주가 끝났을 때 특정 함수 호출
		FScriptDelegate onMontageEndDelegate;
		onMontageEndDelegate.BindUFunction(this, strFunctionName);
		m_pAnim->OnMontageEnded.Add(onMontageEndDelegate);

	}
	else
	{
		m_pAnim->Montage_Play(pMontage);

		// 바로 콜백 바인딩
		FScriptDelegate onMontageEndDelegate;
		onMontageEndDelegate.BindUFunction(this, strFunctionName);
		m_pAnim->OnMontageEnded.Add(onMontageEndDelegate);
	}
}

void UC_GimmickComponent::excuteGimmick()
{
	m_bGimmickPlaying = true;

	m_pMonster->setPhaseState(E_MonsterPhaseState::GimmickExecute);

	m_pAnim->Montage_Play(m_pGimmikPlayMontage);

}


float UC_GimmickComponent::getGimmickTime() const
{
	return m_fGimmickTime;
}

bool UC_GimmickComponent::isMonsterIdle() const
{
	if (m_pMonster->getCurrentState() != E_MonsterPhaseState::Idle)
		return false;

	return true;
}

bool UC_GimmickComponent::canPlayGimmickMontage() const
{
	if (!m_pAnim)
		return false;

	if (m_pMonster->getIsAttacking())
		return false;

	if (m_pMonster->getCurrentState() == E_MonsterPhaseState::Stagger)
		return false;


	return isMonsterIdle();
}

bool UC_GimmickComponent::IsPlayingGimmick()
{
	return m_bGimmickPlaying;
}

void UC_GimmickComponent::endGimmick()
{
	m_bGimmickPlaying = false;

	m_pAnim->Montage_Stop(0.1f);

	m_pAnim->Montage_Play(m_pGimmikEndMontage);

	m_pMonster->setActivePower(false);

	if (m_pMonster->getIsPower() == false)
		m_pMonster->setPhaseState(E_MonsterPhaseState::Idle);

}


// Called every frame
void UC_GimmickComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_bGimmickPlaying)
	{
		m_fGimmickTime -= DeltaTime;
		if (m_fGimmickTime <= 0)
			endGimmick();
	}
		

	// ...
}

