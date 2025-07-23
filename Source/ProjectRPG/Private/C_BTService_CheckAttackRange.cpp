// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BTService_CheckAttackRange.h"

UC_BTService_CheckAttackRange::UC_BTService_CheckAttackRange()
{
	NodeName = TEXT("Check Attack Range");
	Interval = 0.3f;
	RandomDeviation = 0.1f;
}

void UC_BTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* pAiController = OwnerComp.GetAIOwner();
	UBlackboardComponent* pBb = OwnerComp.GetBlackboardComponent();
	if (!pAiController || !pBb)
		return;

	APawn* pAiPawn = pAiController->GetPawn();
	AActor* pTarget = Cast<AActor>(pBb->GetValueAsObject("TargetActor"));

	if (!pAiPawn || !pTarget)
		return;

	float fDistance = FVector::Dist(pAiPawn->GetActorLocation(), pTarget->GetActorLocation());
	bool bInRange = fDistance <= 300.0f;

	pBb->SetValueAsBool("IsInAttackRange", bInRange);

}
