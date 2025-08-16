// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BTTask_Attack.h"
#include "C_MonsterAiController.h"
#include "C_MonsterBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UC_BTTask_Attack::UC_BTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UC_BTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AC_MonsterBaseCharacter* pMonster = Cast<AC_MonsterBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!pMonster)
		return EBTNodeResult::Failed;
	ACharacter* pPlayer = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AC_MonsterAiController::TargetActorKey));


	TArray<int32> arrCandidates = pMonster->filterAvailablePatterns();
	if (arrCandidates.Num() == 0)
		return EBTNodeResult::Failed;

	int32 nIndex = pMonster->selectPatternByWeight(arrCandidates);
	if (nIndex == INDEX_NONE)
		return EBTNodeResult::Failed;

	pMonster->playPattern(nIndex);
		return EBTNodeResult::Succeeded;
	
}