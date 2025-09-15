// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_PerfectZoneResult.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRerultAnimEnd);
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
	UPROPERTY(BlueprintAssignable, Category = "PerfectZoneResult")
	FOnRerultAnimEnd OnRerultAnimEnd;
protected:
	UPROPERTY(BlueprintReadWrite)
	bool EndResult = false;
	UFUNCTION(BlueprintCallable)
	void HandleResultAnimEnd();
	
};
