// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_PerfectZoneResult.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_PerfectZoneResult : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "PerfectZone")
	void Show();
protected:
	UPROPERTY(BlueprintReadWrite)
	bool EndResult = false;
};
