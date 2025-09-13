// Fill out your copyright notice in the Description page of Project Settings.


#include "C_SpawnSkillEffectNotify.h"
#include "C_NiagaraUtil.h"
#include "GameFramework/Actor.h"

void UC_SpawnSkillEffectNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !m_pSkillEffect) return;

    AActor* pOwner = MeshComp->GetOwner();
    if (!pOwner) return;

    FVector vSpawnLocation;
    FRotator rSpawnRotation;

    if (m_bAttachToSocket && MeshComp->DoesSocketExist(m_strSocketName))
    {
        vSpawnLocation = MeshComp->GetSocketLocation(m_strSocketName);
        rSpawnRotation = MeshComp->GetSocketRotation(m_strSocketName);
    }
    else
    {
        vSpawnLocation = pOwner->GetActorLocation();
        rSpawnRotation = pOwner->GetActorRotation();
    }

    UC_NiagaraUtil::spawnNiagaraAtLocation(
        pOwner->GetWorld(),
        m_pSkillEffect,
        vSpawnLocation,
        rSpawnRotation,
        m_fSkillLifeTime,
        m_vSkillScale.X
    );
}
