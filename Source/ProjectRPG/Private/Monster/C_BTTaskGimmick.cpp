// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/C_BTTaskGimmick.h"
#include "C_MonsterBaseCharacter.h"
#include "../Public/Monster/C_GimmickComponent.h"

EBTNodeResult::Type UC_BTTaskGimmick::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AC_MonsterBaseCharacter* pMonster = Cast<AC_MonsterBaseCharacter>(OwnerComp.GetAIOwner());

	if (!pMonster)
		return EBTNodeResult::Failed;


    float fHp = pMonster->getHp();
    float fMaxHp = pMonster->getMaxHp();


	return EBTNodeResult::Failed;
}
