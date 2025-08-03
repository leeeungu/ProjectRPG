// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "C_BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "C_MonsterAiController.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTRPG_API AC_MonsterAiController : public AAIController
{
	GENERATED_BODY()

public:
	AC_MonsterAiController();

protected:
	UPROPERTY()
	UBehaviorTree* m_pBtAsset;

	UPROPERTY()
	UBlackboardData* m_pBbAsset;


private:
	UBehaviorTreeComponent* m_pBT;
	UBlackboardComponent* m_pBB;

public:
	virtual void OnPossess(APawn* pMonster) override;

	virtual void OnUnPossess() override;

protected:
	virtual void BeginPlay() override;
};
