// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MonsterBaseCharacter.h"

AC_MonsterBaseCharacter::AC_MonsterBaseCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AC_MonsterAiController::StaticClass();
}

void AC_MonsterBaseCharacter::takeStaggerEvent(float fStagger)
{
	m_pStaggerComp->applyStagger(fStagger);

}

void AC_MonsterBaseCharacter::onStaggerBroken()
{
	/*
	* ����ȭ �ִ� ��Ÿ��
	* AI ����
	*/
}

void AC_MonsterBaseCharacter::onStaggerRecover()
{
	/*
	* AI ����
	*/
}

void AC_MonsterBaseCharacter::playAttackMontage()
{
	if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(m_pAttackMontage))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(m_pAttackMontage);
	}
	
}

void AC_MonsterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (m_eMonsterRank >= E_MonsterRank::Named)
	{
		if (m_pStaggerComp && m_pStaggerComp == GetOwner()->FindComponentByClass<UC_StaggerComponent>())
		{
			m_pStaggerComp->m_onBroken.AddDynamic(this, &AC_MonsterBaseCharacter::onStaggerBroken);

			m_pStaggerComp->m_onRecover.AddDynamic(this, &AC_MonsterBaseCharacter::onStaggerRecover);
		}
	}
	
}
