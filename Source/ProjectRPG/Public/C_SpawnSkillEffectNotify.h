// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "C_SpawnSkillEffectNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_SpawnSkillEffectNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    class UNiagaraSystem* m_pSkillEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FVector m_vSkillScale = FVector(1.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float m_fSkillLifeTime = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    bool m_bAttachToSocket = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FName m_strSocketName = NAME_None;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
