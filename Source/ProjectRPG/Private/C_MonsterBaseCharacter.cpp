// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MonsterBaseCharacter.h"

AC_MonsterBaseCharacter::AC_MonsterBaseCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AC_MonsterAiController::StaticClass();
}

void AC_MonsterBaseCharacter::playMontage()
{
	if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(m_pAttackMontage))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(m_pAttackMontage);
	}
	
}
