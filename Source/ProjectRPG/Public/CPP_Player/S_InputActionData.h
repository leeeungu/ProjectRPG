// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S_InputActionData.generated.h"

UENUM(BlueprintType)
enum class EInputType : uint8
{
    None,
    Period,
    Skill,
    ChargeSkill,
    Item,
    AnimItem
};

UENUM(BlueprintType)
enum class EInputStateType : uint8
{
    Pressed,
    Released,
    Held
};

USTRUCT(BlueprintType)
struct FInputActionData
{
	GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    int32 ActionIndex{};

    UPROPERTY(BlueprintReadWrite)
    EInputType InputType{};

    UPROPERTY(BlueprintReadWrite)
    EInputStateType InputStateType{};
    UPROPERTY(BlueprintReadWrite)
    FVector TargetPoint{};
    
};
