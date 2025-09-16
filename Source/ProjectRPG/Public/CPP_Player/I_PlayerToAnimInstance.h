// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_PlayerToAnimInstance.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UI_PlayerToAnimInstance : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTRPG_API II_PlayerToAnimInstance
{
	GENERATED_BODY()
public:
	virtual void SetAttackMode(bool b) {}
	virtual void SetIsAttackingMode(bool b) {}
	virtual void SetIsDownMode(bool b) {}
	virtual void SetDownPeriodState(bool b) {}
	virtual void SetIsDownMode_DownState(bool b) {}
	
	

};
