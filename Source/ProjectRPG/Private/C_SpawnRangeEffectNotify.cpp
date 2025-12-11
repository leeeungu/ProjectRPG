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

	FVector adjustedOffset = FVector(m_vEffectLocationOffset.Y, m_vEffectLocationOffset.X, m_vEffectLocationOffset.Z);
	

	FTransform socketTransform = MeshComp->GetSocketTransform(m_strSocketName, RTS_World);

	// 2. 로컬 오프셋 → 월드 방향으로 변환
	FVector worldOffset = socketTransform.TransformVector(adjustedOffset);

	// 3. 최종 위치 계산
	FVector vSpawnLocation = socketTransform.GetLocation() + worldOffset;

	FRotator rSpawnRotation = socketTransform.GetRotation().Rotator();
	vSpawnLocation.Z = 0.1f;

	UE_LOG(LogTemp, Warning, TEXT("Forward: %s | Right: %s | Up: %s"),
		*socketTransform.GetRotation().GetForwardVector().ToString(),
		*socketTransform.GetRotation().GetRightVector().ToString(),
		*socketTransform.GetRotation().GetUpVector().ToString());

	UC_NiagaraUtil::spawnNiagaraAtLocation(
		pOwner->GetWorld(),
		m_pRangeEffect,
		vSpawnLocation,
		rSpawnRotation,
		m_fRangeLifeTime,
		m_fRangeScale
	);

}
