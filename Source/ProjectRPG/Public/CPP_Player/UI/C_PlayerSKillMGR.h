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

    // 퍼펙트존 클래스 세팅 (블루프린트에서 지정)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TSubclassOf<UUserWidget> PerfectZoneClass;

    // 실제 생성된 퍼펙트존 포인터
    UPROPERTY()
    UUserWidget* PerfectZoneWidget;

    UFUNCTION(BlueprintCallable, Category = "SkillUI")
    void CreatePerfectZone();



};
