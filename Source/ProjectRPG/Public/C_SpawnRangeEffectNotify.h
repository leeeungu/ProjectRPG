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
    FVector m_vRangeScale = FVector(1.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float m_fRangeLifeTime = 1.5f;

    // true면 Actor 위치에, false면 Socket 위치에
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    bool m_bAttachToRoot = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FName m_strSocketName = NAME_None;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
