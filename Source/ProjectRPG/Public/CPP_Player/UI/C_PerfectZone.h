// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Slate/WidgetTransform.h"
#include "Components/Image.h"
#include "Components/Textblock.h"
#include "C_PerfectZone.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_PerfectZone : public UUserWidget
{
	GENERATED_BODY()
public:
    // 애니메이션 참조
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* Start;
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* Show;
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* NotShow;

    // 애니메이션 시작
    UFUNCTION(BlueprintCallable)
    void PlayProgressAnimation();

    // 애니메이션 종료
    UFUNCTION(BlueprintCallable)
    void StopProgressAnimation();

    void UpdateSecondText(float TimeValue);

private:
    bool IsTick = false;
    float CurrentTime = 0.f;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    virtual void NativeConstruct() override;

protected:
    
    UPROPERTY(meta = (BindWidget))
    UImage* Bar;

    UPROPERTY(meta = (BindWidget))
    UImage* BarBackup;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Second;

    FWidgetTransform InitialBackupTransform;
	
};
