// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BTTaskPatrol.h"
#include "C_MonsterBaseCharacter.h"
#include "C_MonsterAiController.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "NavigationSystem.h"

UC_BTTaskPatrol::UC_BTTaskPatrol()
{
	NodeName = TEXT("Set Patrol Location");
}

EBTNodeResult::Type UC_BTTaskPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AC_MonsterBaseCharacter* pMonster = Cast<AC_MonsterBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
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

	pBBcomp->SetValueAsVector(AC_MonsterAiController::PatrolLocationKey, navRandom.Location);

	return EBTNodeResult::Succeeded;
}
