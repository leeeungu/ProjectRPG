// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BTService_CheckAttackCondition.h"

#include "C_MonsterBaseCharacter.h"
#include "AIController.h"
#include "C_MonsterAiController.h"

UC_BTService_CheckAttackCondition::UC_BTService_CheckAttackCondition()
{
	NodeName = TEXT("Check Attack Condition");

	bNotifyTick = true;
}

void UC_BTService_CheckAttackCondition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AC_MonsterBaseCharacter* pMonster = Cast<AC_MonsterBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!pMonster) 
		return;

	TArray<int32> arrAvailable = pMonster->filterAvailablePatterns();

	bool bCanCheck = arrAvailable.Num() > 0;

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), bCanCheck);

}
