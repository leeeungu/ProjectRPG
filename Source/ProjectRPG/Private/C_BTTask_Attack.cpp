// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BTTask_Attack.h"

UC_BTTask_Attack::UC_BTTask_Attack()
{
	NodeName = TEXT("Attack");
	m_bIsAttacking = false;
	bNotifyTick = true;
}

EBTNodeResult::Type UC_BTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* pAiController = OwnerComp.GetAIOwner();
	AC_MonsterBaseCharacter* pMonster = Cast<AC_MonsterBaseCharacter>(pAiController->GetPawn());

	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//pAiController->SetFocus(pPlayer);


	if (!pMonster && !pPlayer)
		return EBTNodeResult::Failed;
	/*
	* 공격 직전에 플레이어 쪽으로 회전
	*/
	FVector vToPlayer = pPlayer->GetActorLocation() - pMonster->GetActorLocation();
	FRotator rLookAt = vToPlayer.Rotation();
	rLookAt.Pitch = 0.0f;
	rLookAt.Roll = 0.0f;
	pMonster->SetActorRotation(rLookAt);

	pMonster->playAttackMontage();
	m_bIsAttacking = true;


	return EBTNodeResult::InProgress;
}


void UC_BTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (m_bIsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
