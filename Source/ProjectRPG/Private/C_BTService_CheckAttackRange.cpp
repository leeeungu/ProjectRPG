// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BTService_CheckAttackRange.h"

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

	APawn* pAiPawn = pAiController->GetPawn();
	APawn* pTarget = Cast<APawn>(pBb->GetValueAsObject("TargetActor"));

	if (!pAiPawn || !pTarget)
	{
		pBb->SetValueAsBool("IsInAttackRange", false);
		return;
	}
		

	float fDistance = FVector::Dist(pAiPawn->GetActorLocation(), pTarget->GetActorLocation());
	bool bInRange = fDistance <= 200.0f;

	pBb->SetValueAsBool("IsInAttackRange", bInRange);

	UE_LOG(LogTemp, Warning, TEXT("Distance: %f, InRange: %s"), fDistance, bInRange ? TEXT("true") : TEXT("false"));


}
