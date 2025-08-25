// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S_SkillData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSkillData
{
    GENERATED_BODY()
public:
    // 스킬 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FName SkillName;

    // 사용할 애님 몽타주
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    UAnimMontage* SkillMontage = nullptr;

    // 공격력 퍼센트 (100 = 100%)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float AttackPowerMultiplier = 100.f;

    // 스킬 쿨타임 (초)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Cooldown = 0.f;

};
