// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "C_SpawnRangeEffectNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_SpawnRangeEffectNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    class UNiagaraSystem* m_pRangeEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float m_fRangeScale = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FVector m_vEffectLocationOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float m_fRangeLifeTime = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FName m_strSocketName = NAME_None;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
