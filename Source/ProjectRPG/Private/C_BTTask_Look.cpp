// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BTTask_Look.h"
#include "C_MonsterAiController.h"
#include "C_MonsterBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UC_BTTask_Look::UC_BTTask_Look()
{
	NodeName = TEXT("Look");
}

EBTNodeResult::Type UC_BTTask_Look::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AC_MonsterBaseCharacter* pMonster = Cast<AC_MonsterBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!pMonster)
		return EBTNodeResult::Failed;

	ACharacter* pPlayer = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AC_MonsterAiController::TargetActorKey));

	if (!pPlayer)
		return EBTNodeResult::Failed;

	FVector fDir = pPlayer->GetActorLocation() - pMonster->GetActorLocation();
	
	fDir.Normalize();

	float fAngle = 
	FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(fDir, pMonster->GetActorForwardVector())));

	float fRotDir =
		FVector::DotProduct(fDir, pMonster->GetActorRightVector()) > 0.f ? 1.f : -1.f;

	pMonster->AddActorWorldRotation(FRotator(0.f, fAngle * fRotDir, 0.f));

	return EBTNodeResult::Succeeded;
}
