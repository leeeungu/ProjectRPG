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
	int32 nSelectedIndex = arrValidIndex[nRanIndex];


	FS_AttackData& sAttackData = m_arrAttackList[nSelectedIndex];

	if (sAttackData.bIsCool)
		return;

	if (sAttackData.pAttackMontage && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(sAttackData.pAttackMontage))
	{

		GetMesh()->GetAnimInstance()->Montage_Play(sAttackData.pAttackMontage);

		sAttackData.bIsCool = true;



		FTimerHandle sCooldownHandle{};
		GetWorld()->GetTimerManager().SetTimer(sCooldownHandle, FTimerDelegate::CreateUObject(this, &AC_MonsterBaseCharacter::resetAttackCoolTime, nSelectedIndex),
			 sAttackData.fCoolTime, false);
	}
}

void AC_MonsterBaseCharacter::takeStaggerEvent(float fStagger)
{
	if (m_pStaggerComp)
		m_pStaggerComp->applyStagger(fStagger);

}

void AC_MonsterBaseCharacter::onStaggerBroken()
{
	/*
	* 무력화 애님 몽타주
	* AI 정지
	*/
}

void AC_MonsterBaseCharacter::onStaggerRecover()
{
	/*
	* AI 가동
	*/
}

float AC_MonsterBaseCharacter::getMaxVaildAttackRange() const
{
	float fMaxRange{};

	for (const FS_AttackData& sAttackData : m_arrAttackList)
	{
		if (!sAttackData.bIsCool)
			fMaxRange = FMath::Max(fMaxRange, sAttackData.fAttackRange);
	}

	return fMaxRange;
}

bool AC_MonsterBaseCharacter::hasAnyVaildAttack() const
{
	for (const FS_AttackData& sData : m_arrAttackList)
	{
		if (!sData.bIsCool)
			return true;
	}
	return false;
}

bool AC_MonsterBaseCharacter::isPlayingAttackMontage() const
{
	UAnimInstance* pAnim = GetMesh()->GetAnimInstance();

	if (!pAnim)
		return false;

	for (const FS_AttackData& sData : m_arrAttackList)
	{
		if (sData.pAttackMontage && pAnim->Montage_IsPlaying(sData.pAttackMontage))
			return true;
	}
	return false;
}

void AC_MonsterBaseCharacter::resetAttackCoolTime(int32 nIndex)
{
	if (m_arrAttackList.IsValidIndex(nIndex))
		m_arrAttackList[nIndex].bIsCool = false;
}

void AC_MonsterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (m_eMonsterRank >= E_MonsterRank::Named)
	{
		m_pStaggerComp = GetOwner()->FindComponentByClass<UC_StaggerComponent>();

		if (m_pStaggerComp)
		{
			m_pStaggerComp->m_onBroken.AddDynamic(this, &AC_MonsterBaseCharacter::onStaggerBroken);

			m_pStaggerComp->m_onRecover.AddDynamic(this, &AC_MonsterBaseCharacter::onStaggerRecover);
		}
	}

	
}
