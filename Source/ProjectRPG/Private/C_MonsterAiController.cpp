// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MonsterAiController.h"


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

void AC_MonsterAiController::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	
	//m_pBB->SetValueAsObject("TargetActor", pPlayer);

}
