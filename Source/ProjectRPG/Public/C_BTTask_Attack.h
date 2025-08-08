// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "C_MonsterBaseCharacter.h"
#include "C_BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_BTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()

private:
	bool m_bIsAttacking;

public:
	UC_BTTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
