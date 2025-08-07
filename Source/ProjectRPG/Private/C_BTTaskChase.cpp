// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BTTaskChase.h"
#include "C_MonsterBaseCharacter.h"
#include "C_MonsterAiController.h"

UC_BTTaskChase::UC_BTTaskChase()
{
	NodeName = TEXT("Chase Target");
	m_fAcceptableRadius = 100.0f;

	bNotifyTick = false;

}

EBTNodeResult::Type UC_BTTaskChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* pAiController = OwnerComp.GetAIOwner();
	if (!pAiController)
	{
		return EBTNodeResult::Failed;
	}
	AC_MonsterBaseCharacter* pMonster = Cast<AC_MonsterBaseCharacter>(pAiController->GetPawn());

	UBlackboardComponent* pBBcomp = OwnerComp.GetBlackboardComponent();
	APawn* pTarget = Cast<APawn>(pBBcomp->GetValueAsObject(m_sTargetActorKey.SelectedKeyName));

	if (!pTarget)
	{
		return EBTNodeResult::Failed;
	}

	EPathFollowingRequestResult::Type moveResult{};

	moveResult = pAiController->MoveToActor(pTarget,
		m_fAcceptableRadius, true, true, true, 0, true);


		
	if (moveResult == EPathFollowingRequestResult::Failed)
	{
		return EBTNodeResult::Failed;
	}


	float Distance = FVector::Dist(pMonster->GetActorLocation(), pTarget->GetActorLocation());
	if (Distance <= pMonster->getMaxVaildAttackRange())
	{
		UE_LOG(LogTemp, Warning, TEXT("Close enough ¡æ Chase ends"));
		return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::InProgress;
}
