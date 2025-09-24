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

	UE_LOG(LogTemp, Warning, TEXT("[PhaseChange] CurrentHp: %.1f / %.1f, Index: %d"), fHp, fMaxHp, m_nCurrentPhaseIndex);

	if (fHp <= 0 || fMaxHp <= 0)
		return;

	if (m_arrPhase.Num() == 0)
		return;

	if (m_nCurrentPhaseIndex >= m_arrPhase.Num())
		return;

	

	float fCurrentHpPercent = (fHp / fMaxHp) * 100.f;

	const FS_PhaseData& sPhase = m_arrPhase[m_nCurrentPhaseIndex];
	

	if (fCurrentHpPercent <= sPhase.fChangePercentHp)
	{
		if (!m_pAnim)
			return;

		m_pMonster->setPhaseState(E_MonsterPhaseState::PhaseChanging);

		if (m_pAnim->IsAnyMontagePlaying() || m_pMonster->getCurrentState() == E_MonsterPhaseState::Stagger)
		{
			m_pAnim->OnMontageEnded.AddDynamic(this, &UC_PhaseComponent::OnMontageEnded_PhaseChange);
			return;	
		}
		
		m_pMonster->setPhaseState(E_MonsterPhaseState::PhaseChanged);
		m_onPhaseChange.Broadcast();

		//m_pMonster->reservePhaseChange(fHp, fMaxHp);

		
	}
	else
		return;




	//if (!m_pMonster || m_pMonster->getCurrentState() != E_MonsterPhaseState::Idle)
	//{
	//	// 공격 중이면 예약
	//	if (m_pMonster)
	//	{
	//		m_pMonster->bPendingPhaseChange = true;
	//		m_pMonster->m_fPendingHp = fHp;
	//		m_pMonster->m_fPendingMaxHp = fMaxHp;

	//		UE_LOG(LogTemp, Warning, TEXT("PhaseChange called! fPendingHp=%f, fPendingMaxHp=%f, Index=%d, State=%d"),
	//			m_pMonster->m_fPendingHp, m_pMonster->m_fPendingMaxHp, m_nCurrentPhaseIndex,
	//			m_pMonster ? (int32)m_pMonster->getCurrentState() : -1);
	//	}
	//	return;
	//}
	
}

void UC_PhaseComponent::OnMontageEnded_PhaseChange(UAnimMontage* Montage, bool bInterrupted)
{
	if (m_pAnim && m_pAnim->GetCurrentActiveMontage() == Montage)
		return;


	if (m_pAnim)
	{
		m_pAnim->OnMontageEnded.RemoveDynamic(this, &UC_PhaseComponent::OnMontageEnded_PhaseChange);
	}


	if (m_nCurrentPhaseIndex >= m_arrPhase.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnMontageEnded_PhaseChange called but index out of range. Index=%d, Num=%d"),
			m_nCurrentPhaseIndex, m_arrPhase.Num());
		return;
	}

	const FS_PhaseData& sPhase = m_arrPhase[m_nCurrentPhaseIndex];

	if (m_nCurrentPhaseIndex < m_arrPhase.Num())
	{
		m_pMonster->setPhaseState(E_MonsterPhaseState::PhaseChanged);
		m_onPhaseChange.Broadcast();

		m_pMonster->tryTriggerPhaseChangeOrGimmick();
	}


	m_nCurrentPhaseIndex++;
	
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

