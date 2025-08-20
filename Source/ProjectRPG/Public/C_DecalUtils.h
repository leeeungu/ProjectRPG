// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "C_DecalUtils.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_DecalUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Decal")
	static void spawnDecalAtLocation(UWorld* pWorld, UMaterialInterface* pMat, FVector vLocation,
		FVector vSize, float fDuration);
	
};
