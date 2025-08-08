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

	AAIController* pAiController = OwnerComp.GetAIOwner();
	UBlackboardComponent* pBb = OwnerComp.GetBlackboardComponent();
	if (!pAiController || !pBb)
		return;

	AC_MonsterBaseCharacter* pMonster = Cast<AC_MonsterBaseCharacter>(pAiController->GetPawn());
	APawn* pTarget = Cast<APawn>(pBb->GetValueAsObject("TargetActor"));

	


	if (!pMonster || !pTarget)
	{
		pBb->SetValueAsBool("CanAttack", false);
		return;
	}

	float fDistance = FVector::Dist(pMonster->GetActorLocation(), pTarget->GetActorLocation());

	bool bHasVaildAttack = pMonster->hasAnyVaildAttack();
	bool bInRange = fDistance <= pMonster->getMaxVaildAttackRange();
	bool bNotBusy = !pMonster->isPlayingAttackMontage();

	bool bCanAttack = bHasVaildAttack && bInRange && bNotBusy;

	pBb->SetValueAsBool("CanAttack", bCanAttack);

}
