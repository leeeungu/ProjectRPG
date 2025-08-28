// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/C_GimmickComponent.h"
#include "C_StaggerGimmickComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaggerGimmickStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaggerGimmickEnd);

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTRPG_API UC_StaggerGimmickComponent : public UC_GimmickComponent
{
	GENERATED_BODY()

private:
	float m_fGoalStagger = 200.f;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStaggerGimmickStart m_onStaggerGimmickStart;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStaggerGimmickEnd m_onStaggerGimmickEnd;

	float getGoalStagger() const;


protected:
	virtual bool canGimmickStart(float fHp, float fMaxHp) override;

	virtual void excuteGimmick() override;

	virtual bool endGimmick() override; 

public:
	UC_StaggerGimmickComponent();
	
};
