// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MonsterAiController.h"

AC_MonsterAiController::AC_MonsterAiController()
{
	m_pBT = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	m_pBB = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
}

void AC_MonsterAiController::OnPossess(APawn* pMonster)
{
}

void AC_MonsterAiController::OnUnPossess()
{
}

void AC_MonsterAiController::BeginPlay()
{
	Super::BeginPlay();

	APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	SetFocus(pPlayerPawn);
}
