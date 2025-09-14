// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "C_SpawnSkillTrailNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_SpawnSkillTrailNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:
    // 사용할 Niagara System
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    class UNiagaraSystem* m_pTrailEffect;

    // Niagara Component 스케일
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FVector m_vTrailScale = FVector(1.f);

    // 소켓에 붙일지 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    bool m_bAttachToSocket = true;

    // 붙일 소켓 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FName m_strSocketName = NAME_None;

private:
    // 스폰한 Niagara 컴포넌트
    UPROPERTY(Transient)
    class UNiagaraComponent* m_pSpawnedTrail;

public:
    //virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    //virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
