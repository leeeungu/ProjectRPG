// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/C_GimmickComponent.h"
#include "C_StaggerGimmickComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_StaggerGimmickComponent : public UC_GimmickComponent
{
	GENERATED_BODY()


protected:
	virtual bool canGimmickStart() override;

	virtual void excuteGimmick() override;

public:
	UC_StaggerGimmickComponent();
	
};
