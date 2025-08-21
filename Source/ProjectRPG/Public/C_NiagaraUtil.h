// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraFunctionLibrary.h"
#include "C_NiagaraUtil.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_NiagaraUtil : public UNiagaraFunctionLibrary
{
	GENERATED_BODY()
public:
	static void spawnNiagaraAtLocation(UWorld* pWorld, UNiagaraSystem* pNiagaraSys, FVector vLocation,
		FRotator rRot, float fLifeTime, float fScale);
	
};
