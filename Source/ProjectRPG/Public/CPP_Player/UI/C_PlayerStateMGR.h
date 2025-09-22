// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_PlayerStateMGR.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_PlayerStateMGR : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION()
	void OnPlayerDead();
	
};
