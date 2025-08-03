// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "C_BaseCharacter.h"
#include "C_MonsterAiController.h"
#include "NavigationSystem.h"
#include "C_BTTaskPatrol.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_BTTaskPatrol : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UC_BTTaskPatrol() = default;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
