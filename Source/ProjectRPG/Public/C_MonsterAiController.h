// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "C_MonsterAiController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API AC_MonsterAiController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UBehaviorTreeComponent* m_pBT;
	UPROPERTY()
	class UBlackboardComponent* m_pBB;

protected:
	UPROPERTY()
	class UBehaviorTree* m_pBtAsset;

	UPROPERTY()
	class UBlackboardData* m_pBbAsset;

public:
	static const FName TargetActorKey;
	static const FName CanAttackKey;
	static const FName PatrolLocationKey;
	static const FName IsGimmickPlayingKey;

public:
	AC_MonsterAiController();

	virtual void OnPossess(APawn* pMonster) override;

	virtual void OnUnPossess() override;

	void restartAi();

protected:
	virtual void BeginPlay() override;
};
