// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "C_AnimNotify_AOE.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_AnimNotify_AOE : public UAnimNotify
{
	GENERATED_BODY()
public:
    // 소환할 나이아가라 이펙트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE Effect")
    class UNiagaraSystem* NiagaraEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE Effect")
    FName AOE_SocketName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE Effect")
    FVector AOE_LocationOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE Effect")
    FRotator AOE_RotationOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE Effect")
    FVector AOE_Scale = FVector(1.0f, 1.0f, 1.0f);

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
