// Fill out your copyright notice in the Description page of Project Settings.


#include "C_SpawnRangeEffectNotify.h"
#include "GameFramework/Actor.h"
#include "C_NiagaraUtil.h"

void UC_SpawnRangeEffectNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp || !m_pRangeEffect)
		return;

	AActor* pOwner = MeshComp->GetOwner();
	if (!pOwner)
		return;

	FVector vSpawnLocation = m_bAttachToRoot ? pOwner->GetActorLocation() : MeshComp->GetSocketLocation(m_strSocketName);
	FRotator rSpawnRotation = m_bAttachToRoot ? pOwner->GetActorRotation() : MeshComp->GetSocketRotation(m_strSocketName);
	vSpawnLocation.Z = 0.1f;

	UC_NiagaraUtil::spawnNiagaraAtLocation(
		pOwner->GetWorld(),
		m_pRangeEffect,
		vSpawnLocation,
		rSpawnRotation,
		m_fRangeLifeTime,
		m_vRangeScale.X
	);

}
