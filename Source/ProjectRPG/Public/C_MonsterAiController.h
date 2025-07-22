// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "C_MonsterBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackBoardComponent.h"
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

	virtual void OnPossess(APawn* pMonster) override;

	virtual void OnUnPossess() override;

protected:
	virtual void BeginPlay() override;

private:
	UBehaviorTreeComponent* m_pBT;
	UBlackboardComponent* m_pBB;
	
};
