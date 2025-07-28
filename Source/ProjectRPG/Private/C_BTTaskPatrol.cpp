// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BTTaskPatrol.h"

EBTNodeResult::Type UC_BTTaskPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* pMonster = OwnerComp.GetAIOwner()->GetCharacter();
	if (!pMonster)
		return EBTNodeResult::Failed;
	
	UNavigationSystemV1* pNavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!pNavSys)
		return EBTNodeResult::Failed;

	FNavLocation navRandom{};
	float fRadius = 800.0f;

	bool bFound = pNavSys->GetRandomReachablePointInRadius(pMonster->GetActorLocation(), fRadius, navRandom);
	if (!bFound)
		return EBTNodeResult::Failed;

	UBlackboardComponent* pBBcomp = OwnerComp.GetBlackboardComponent();
	if (!pBBcomp)
		return EBTNodeResult::Failed;

	pBBcomp->SetValueAsVector("PatrolLocation", navRandom.Location);


	return EBTNodeResult::Succeeded;
}
