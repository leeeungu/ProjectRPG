// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BTService_CheckAttackRange.h"

#include "C_MonsterBaseCharacter.h"
#include "AIController.h"
#include "C_MonsterAiController.h"

UC_BTService_CheckAttackRange::UC_BTService_CheckAttackRange()
{
	NodeName = TEXT("Check Attack Range");
	Interval = 0.3f;
	RandomDeviation = 0.1f;

	bNotifyTick = true;
}

void UC_BTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
		pBb->SetValueAsBool("IsInAttackRange", false);
		return;
	}

	float fAttackRange = pMonster->getAttackRange();

	float fDistance = FVector::Dist(pMonster->GetActorLocation(), pTarget->GetActorLocation());
	bool bInRange = fDistance <= fAttackRange;

	pBb->SetValueAsBool("IsInAttackRange", bInRange);

	//UE_LOG(LogTemp, Warning, TEXT("attackRange: %.f"), fAttackRange);


}
