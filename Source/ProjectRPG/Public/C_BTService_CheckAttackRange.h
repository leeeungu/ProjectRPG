// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "C_BTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_BTService_CheckAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UC_BTService_CheckAttackRange();

protected:
	/**
	* ���񽺰� Ȱ��ȭ�� ��ο� ������ �׻� ������Ʈ�� ����.
	*/
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
