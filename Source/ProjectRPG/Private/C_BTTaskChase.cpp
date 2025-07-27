// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BTTaskChase.h"

UC_BTTaskChase::UC_BTTaskChase()
{
	NodeName = TEXT("Chase Target");
	m_fAcceptableRadius = 5.0f;
	bNotifyTick = false;
}

EBTNodeResult::Type UC_BTTaskChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* pAiController = OwnerComp.GetAIOwner();
	if (!pAiController)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* pBb = OwnerComp.GetBlackboardComponent();
	APawn* pTarget = Cast<APawn>(pBb->GetValueAsObject(m_sTargetActorKey.SelectedKeyName));

	if (!pTarget)
	{
		return EBTNodeResult::Failed;
	}

	EPathFollowingRequestResult::Type moveResult = pAiController->MoveToActor(pTarget,
		m_fAcceptableRadius, true, true, true, 0, true);

	if (moveResult == EPathFollowingRequestResult::Failed)
	{
		return EBTNodeResult::Failed;
	}


	float Distance = FVector::Dist(pAiController->GetPawn()->GetActorLocation(), pTarget->GetActorLocation());
	if (Distance <= m_fAcceptableRadius)
	{
		UE_LOG(LogTemp, Warning, TEXT("Close enough ¡æ Chase ends"));
		return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::InProgress;
}
