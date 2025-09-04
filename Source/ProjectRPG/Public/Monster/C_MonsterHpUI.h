// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_MonsterHpUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_MonsterHpUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TArray<class UProgressBar*> m_arrHpBars;

	UPROPERTY(BlueprintReadWrite, Category = "Hp")
	float m_fSegMentHp{};


	UFUNCTION(BlueprintCallable, Category = "Hp")
	void updateHpBars(float fCurHp, float fMaxHp);
};
