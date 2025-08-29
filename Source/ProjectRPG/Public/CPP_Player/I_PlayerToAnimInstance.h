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
class II_PlayerToAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetAttackMode(bool b);

};
