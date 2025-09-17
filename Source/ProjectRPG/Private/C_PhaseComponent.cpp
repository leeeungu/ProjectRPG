// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PhaseComponent.h"
#include "C_MonsterBaseCharacter.h"

// Sets default values for this component's properties
UC_PhaseComponent::UC_PhaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UC_PhaseComponent::phaseChange(float fHp, float fMaxHp)
{
	if (fHp <= 0 || fMaxHp <= 0)
		return;

	if (m_arrPhase.Num() == 0)
		return;

	if (m_nCurrentPhaseIndex >= m_arrPhase.Num())
		return;

	if (!m_pMonster || m_pMonster->getCurrentState() != E_MonsterPhaseState::Idle)
	{
		// 공격 중이면 예약
		if (m_pMonster)
		{
			m_pMonster->bPendingPhaseChange = true;
			m_pMonster->fPendingHp = fHp;
			m_pMonster->fPendingMaxHp = fMaxHp;
		}
		return;
	}

	const FS_PhaseData& sPhase = m_arrPhase[m_nCurrentPhaseIndex];
	

	float fCurrentHpPercent = (fHp / fMaxHp ) * 100.f;

	if (fCurrentHpPercent <= sPhase.fChangePercentHp)
	{
		if (!m_pAnim)
			return;

		m_pMonster->setPhaseState(E_MonsterPhaseState::PhaseChanging);

		if (m_pAnim->IsAnyMontagePlaying())
		{
			m_pAnim->OnMontageEnded.AddDynamic(this, &UC_PhaseComponent::OnMontageEnded_PhaseChange);
			return;	
		}
		m_nCurrentPhaseIndex++;
		m_pMonster->setPhaseState(E_MonsterPhaseState::PhaseChanged);
		m_onPhaseChange.Broadcast();

		m_pMonster->tryTriggerPhaseChangeOrGimmick();
		
	}
	else
		return;
	
}

void UC_PhaseComponent::OnMontageEnded_PhaseChange(UAnimMontage* Montage, bool bInterrupted)
{
	if (m_pAnim)
	{
		m_pAnim->OnMontageEnded.RemoveDynamic(this, &UC_PhaseComponent::OnMontageEnded_PhaseChange);
	}

	const FS_PhaseData& sPhase = m_arrPhase[m_nCurrentPhaseIndex];

	if (Montage == sPhase.pPhaseMontage)
		return;

	m_nCurrentPhaseIndex++;
	m_pMonster->setPhaseState(E_MonsterPhaseState::PhaseChanged);
	m_onPhaseChange.Broadcast();
	
}

// Called when the game starts
void UC_PhaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	m_pMonster = Cast<AC_MonsterBaseCharacter>(GetOwner());
	if (m_pMonster)
	{
		m_pAnim = m_pMonster->GetMesh()->GetAnimInstance();
	}
	
}


// Called every frame
void UC_PhaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

