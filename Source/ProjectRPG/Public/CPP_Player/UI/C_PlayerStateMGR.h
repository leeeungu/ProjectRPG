// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_PlayerStateMGR.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRestartButtonClick);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEXITButtonClick);
/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_PlayerStateMGR : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
	void RestartPlayerGoing();
	UFUNCTION(BlueprintCallable)
	void ExitGoing();
	
public:
	UFUNCTION()
	void OnPlayerDead();

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Restart")
	FOnRestartButtonClick OnRestartButtonClick;

	UPROPERTY(BlueprintAssignable, Category = "Restart")
	FOnEXITButtonClick OnEXITButtonClick;
	

	
};
