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

private:
	TArray<class UProgressBar*> m_arrHpBars;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HpBar_1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HpBar_2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HpBar_3;

protected:
	virtual void NativeConstruct() override;

	

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Hp")
	float m_fSegMentHp{};


	UFUNCTION(BlueprintCallable, Category = "Hp")
	void updateHpBars(float fCurHp, float fMaxHp);
};
