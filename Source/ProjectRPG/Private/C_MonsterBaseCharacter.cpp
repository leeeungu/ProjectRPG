// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MonsterBaseCharacter.h"

AC_MonsterBaseCharacter::AC_MonsterBaseCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AC_MonsterAiController::StaticClass();
}

void AC_MonsterBaseCharacter::playAttackMontage()
{
	if (m_arrAttackList.Num() == 0)
		return;

	TArray<int32> arrValidIndex;
	for (int32 i = 0; i < m_arrAttackList.Num(); i++)
	{
		if (!m_arrAttackList[i].bIsCool)
		{
			arrValidIndex.Add(i);
		}
	}

	if (arrValidIndex.Num() == 0)
		return;



	int32 nRanIndex = FMath::RandRange(0, arrValidIndex.Num() - 1);
	m_nCurrentAttackIndex = nRanIndex;

	FS_AttackData& sAttackData = m_arrAttackList[nRanIndex];
	if (sAttackData.bIsCool)
		return;

	if (sAttackData.pAttackMontage && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(sAttackData.pAttackMontage))
	{

		GetMesh()->GetAnimInstance()->Montage_Play(sAttackData.pAttackMontage);

		sAttackData.bIsCool = true;
		UE_LOG(LogTemp, Warning, TEXT("CoolTime Start: %s, duration: %.2f��"), *sAttackData.strAttackName, sAttackData.fCoolTime);
		FTimerHandle sCooldownHandle{};
		GetWorld()->GetTimerManager().SetTimer(sCooldownHandle, FTimerDelegate::CreateLambda([&sAttackData]() {
			sAttackData.bIsCool = false;
			UE_LOG(LogTemp, Warning, TEXT("CoolTime end: %s"), *sAttackData.strAttackName);

			}), sAttackData.fCoolTime, false);
		

	}
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

float AC_MonsterBaseCharacter::getAttackRange() const
{
	return m_arrAttackList[m_nCurrentAttackIndex].fAttackRange;
}

void AC_MonsterBaseCharacter::startAttackCoolTime(FS_AttackData& sAttackData)
{
	if (sAttackData.bIsCool)
		return;

	sAttackData.bIsCool = true;

	GetWorld()->GetTimerManager().SetTimer(m_timeHandle, this, &AC_MonsterBaseCharacter::resetAttackCoolTime, sAttackData.fCoolTime, false);

}

void AC_MonsterBaseCharacter::resetAttackCoolTime()
{
	m_arrAttackList[m_nCurrentAttackIndex].bIsCool = false;
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
