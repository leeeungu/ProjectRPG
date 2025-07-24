// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BTTask_Attack.h"

EBTNodeResult::Type UC_BTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* pAiController = OwnerComp.GetAIOwner();
	AC_MonsterBaseCharacter* pMonster = Cast<AC_MonsterBaseCharacter>(pAiController->GetPawn());

	if (pMonster)
	{
		pMonster->playMontage();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
