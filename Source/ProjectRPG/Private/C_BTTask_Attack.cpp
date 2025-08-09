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

	AC_MonsterBaseCharacter* pMonster = Cast<AC_MonsterBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!pMonster)
		return EBTNodeResult::Failed;
	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);


	TArray<int32> arrCandidates = pMonster->filterAvailablePatterns();
	if (arrCandidates.Num() == 0)
		return EBTNodeResult::Failed;

	int32 nIndex = pMonster->selectPatternByWeight(arrCandidates);
	if (nIndex == INDEX_NONE)
		return EBTNodeResult::Failed;

	/*
	* 공격 직전에 플레이어 쪽으로 회전
	*/
	FVector vToPlayer = pPlayer->GetActorLocation() - pMonster->GetActorLocation();
	FRotator rLookAt = vToPlayer.Rotation();
	rLookAt.Pitch = 0.0f;
	rLookAt.Roll = 0.0f;

	FQuat qCurLook = pMonster->GetViewRotation().Quaternion();
	qCurLook = FQuat::Slerp(qCurLook, rLookAt.Quaternion(), 7.f * GetWorld()->GetDeltaSeconds());
	pMonster->SetActorRotation(qCurLook);

	pMonster->playPattern(nIndex);
		return EBTNodeResult::Succeeded;
	
}