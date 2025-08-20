// Fill out your copyright notice in the Description page of Project Settings.


#include "C_DecalUtils.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"

void UC_DecalUtils::spawnDecalAtLocation(UWorld* pWorld, UMaterialInterface* pMat, FVector vLocation, FVector vSize, float fDuration)
{
	if (!pWorld || !pMat)
		return;

	FRotator rRot = FRotator(-90.f, 0.f, 0.f);

	ADecalActor* pDecal = pWorld->SpawnActor<ADecalActor>(vLocation, rRot);

	if (pDecal)
	{
		UDecalComponent* pDecalCom = pDecal->GetDecal();

		if (pDecalCom)
		{
			pDecal->SetDecalMaterial(pMat);
			pDecalCom->DecalSize = vSize;
		}

		pDecal->SetLifeSpan(fDuration);		
		
	}
	
	
}
