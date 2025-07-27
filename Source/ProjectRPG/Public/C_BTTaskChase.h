// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "C_MonsterAiController.h"
#include "Navigation/PathFollowingComponent.h"
#include "C_BTTaskChase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_BTTaskChase : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UC_BTTaskChase();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector m_sTargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float m_fAcceptableRadius;
};
