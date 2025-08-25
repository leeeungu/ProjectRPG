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
    // ��ų �̸�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FName SkillName;

    // ����� �ִ� ��Ÿ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    UAnimMontage* SkillMontage = nullptr;

    // ���ݷ� �ۼ�Ʈ (100 = 100%)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float AttackPowerMultiplier = 100.f;

    // ��ų ��Ÿ�� (��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Cooldown = 0.f;

};
