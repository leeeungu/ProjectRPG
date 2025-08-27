// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/C_GimmickComponent.h"
#include "C_StaggerGimmickComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaggerGimmick);

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTRPG_API UC_StaggerGimmickComponent : public UC_GimmickComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStaggerGimmick m_onStaggerGimmick;


protected:
	virtual bool canGimmickStart(float fHp, float fMaxHp) override;

	virtual void excuteGimmick() override;

public:
	UC_StaggerGimmickComponent();
	
};
