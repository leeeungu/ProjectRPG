// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_PlayerSKillMGR.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_PlayerSKillMGR : public UUserWidget
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "SkillUI")
    void ShowPerfectZone();
    void InitPerfectZone();
    UFUNCTION()
    void HiddenPerfectZone();

    void InitResultWidgets();
    UFUNCTION()
    void ShowResult(bool result);
    UFUNCTION()
    void OnNotShowAnimFinished();

    UFUNCTION()
    void FailEnd();
    UFUNCTION()
    void SuccessEnd();

protected:
    virtual void NativeConstruct() override;
    // 퍼펙트존 클래스 세팅 (블루프린트에서 지정)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TSubclassOf<UUserWidget> PerfectZoneClass;

    // 실제 생성된 퍼펙트존 포인터
    UPROPERTY()
    class UC_PerfectZone* PerfectZoneWidget;

    bool EndPerfectZone = false;

    //Result
    UPROPERTY(EditAnywhere, Category = "PerfectZone|Result")
    TSubclassOf<class UC_PerfectZoneResult> SuccessWidgetClass;

    UPROPERTY(EditAnywhere, Category = "PerfectZone|Result")
    TSubclassOf<UC_PerfectZoneResult> FailWidgetClass;

    UPROPERTY()
    UC_PerfectZoneResult* SuccessWidgetInstance;

    UPROPERTY()
    UC_PerfectZoneResult* FailWidgetInstance;
    



};
