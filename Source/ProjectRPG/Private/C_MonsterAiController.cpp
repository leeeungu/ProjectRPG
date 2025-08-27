// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MonsterAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

const FName AC_MonsterAiController::TargetActorKey = TEXT("TargetActor");
const FName AC_MonsterAiController::CanAttackKey = TEXT("CanAttack");
const FName AC_MonsterAiController::PatrolLocationKey = TEXT("PatrolLocation");


AC_MonsterAiController::AC_MonsterAiController()
{
	m_pBT = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	m_pBB = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB_object(TEXT("/Game/Character/Monster/AI/BB_Monster.BB_Monster"));
	if (BB_object.Succeeded())
	{
		m_pBbAsset = BB_object.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_object(TEXT("/Game/Character/Monster/AI/BT_Monster.BT_Monster"));
	if (BT_object.Succeeded())
	{
		m_pBtAsset = BT_object.Object;
	}


}

void AC_MonsterAiController::OnPossess(APawn* pMonster)
{
	Super::OnPossess(pMonster);
	

	if (m_pBbAsset && m_pBtAsset)
	{

		if (UseBlackboard(m_pBbAsset, m_pBB))
		{

			RunBehaviorTree(m_pBtAsset);
		
		}
	}

}

void AC_MonsterAiController::OnUnPossess()
{
}

void AC_MonsterAiController::restartAi()
{
	if (m_pBbAsset && m_pBtAsset)
	{

		if (UseBlackboard(m_pBbAsset, m_pBB))
		{

			RunBehaviorTree(m_pBtAsset);

		}
	}
}

void AC_MonsterAiController::BeginPlay()
{
	Super::BeginPlay();

	

}
