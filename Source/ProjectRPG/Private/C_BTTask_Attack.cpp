// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BTTask_Attack.h"

UC_BTTask_Attack::UC_BTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UC_BTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* pAiController = OwnerComp.GetAIOwner();
	AC_MonsterBaseCharacter* pMonster = Cast<AC_MonsterBaseCharacter>(pAiController->GetPawn());

	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	pAiController->SetFocus(pPlayer);
	

	if (pMonster && pPlayer)
	{
		/*
		* 공격 직전에 플레이어 쪽으로 회전
		*/
		FVector vToPlayer = pPlayer->GetActorLocation() - pMonster->GetActorLocation();
		FRotator rLookAt = vToPlayer.Rotation();
		rLookAt.Pitch = 0.0f;
		rLookAt.Roll = 0.0f;
		pMonster->SetActorRotation(rLookAt);
		
		pMonster->playAttackMontage();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
