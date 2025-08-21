// Fill out your copyright notice in the Description page of Project Settings.


#include "C_NiagaraUtil.h"
#include "NiagaraComponent.h"

void UC_NiagaraUtil::spawnNiagaraAtLocation(UWorld* pWorld, UNiagaraSystem* pNiagaraSys, FVector vLocation, FRotator rRot
	,float fLifeTime, float fScale)
{
	if (!pWorld || !pNiagaraSys)
		return;

	UNiagaraComponent* pNiagaraCom = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		pWorld,
		pNiagaraSys,
		vLocation, 
		rRot, 
		FVector(1.f),
		true);

	if (pNiagaraCom)
	{
		pNiagaraCom->SetVariableFloat(FName("User.Lifetime"), fLifeTime);
		pNiagaraCom->SetVariableFloat(FName("User.Scale"), fScale);
	}

}
