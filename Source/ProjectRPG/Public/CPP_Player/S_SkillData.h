// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S_SkillData.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESkillCollisionShapeType : uint8
{
    Sphere,
    Box,
    Capsule
};

UENUM(BlueprintType)
enum class E4WayDirection : uint8
{
    Forward,
    Back,
    Left,
    Right,
    Default
};

USTRUCT(BlueprintType)
struct FSkillCollisionData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESkillCollisionShapeType ShapeType = ESkillCollisionShapeType::Sphere;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Dimensions = FVector(100.f); // Sphere: X=radius, Box: XYZ, Capsule: X=radius, Z=half-height

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector RelativeOffset = FVector::ForwardVector * 200.f; // 캐릭터 기준 전방 200cm

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration = 0.1f; // 콜리전 유지 시간

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bApplyDamage = true;
};

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

    // 사용할 애님 몽타주(방향 있음)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<E4WayDirection, UAnimMontage*> DirectionMontages;

    // 공격력 퍼센트 (100 = 100%)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float AttackPowerMultiplier = 100.f;

    // 스킬 쿨타임 (초)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Cooldown = 0.f;

    // 스킬 카운터여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    bool Counter = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FSkillCollisionData CollisionData;

};
