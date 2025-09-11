// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "C_PerfectZone.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_PerfectZone : public UUserWidget
{
	GENERATED_BODY()
public:
    // 애니메이션 시작
    UFUNCTION(BlueprintCallable)
    void PlayProgressAnimation();

    // 애니메이션 종료
    UFUNCTION(BlueprintCallable)
    void StopProgressAnimation();

private:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
    // 애니메이션 참조
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* Start;
    UPROPERTY(meta = (BindWidget))
    UImage* Bar;

    UPROPERTY(meta = (BindWidget))
    UImage* BarBackup;
	
};
