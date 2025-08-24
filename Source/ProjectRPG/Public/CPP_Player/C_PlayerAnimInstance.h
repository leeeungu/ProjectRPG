// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	bool IsMove{};
};
