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
    PlainAttack,
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ActionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EInputType InputType{};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EInputStateType InputStateType{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetPoint{};

    UPROPERTY(BlueprintReadWrite)
    float Timestamp = 0.f;

    UPROPERTY(BlueprintReadWrite)
    bool Timing = true;
    
};
