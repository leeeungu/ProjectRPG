// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SkillHit.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API USkillHit : public UAnimNotify
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillHit")
    int32 CollisionIndex = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillHit")
    FName SocketName = TEXT("hit_Collision");
    // 소켓 위치에 더할 위치 오프셋 (로컬좌표 기준)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillHit")
    FVector CollisionOffset = FVector::ZeroVector;

    // 소켓 회전에 더할 회전 오프셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillHit")
    FRotator CollisionRotationOffset = FRotator::ZeroRotator;

    virtual FString GetNotifyName_Implementation() const override
    {
        return FString::Printf(TEXT("SkillHit[%d]"), CollisionIndex);
    }
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
