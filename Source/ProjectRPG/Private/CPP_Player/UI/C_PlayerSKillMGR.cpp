// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/UI/C_PlayerSKillMGR.h"
#include "CPP_Player/UI/C_PerfectZone.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"



void UC_PlayerSKillMGR::ShowPerfectZone()
{
    CreatePerfectZone();
}

void UC_PlayerSKillMGR::CreatePerfectZone()
{
    if (!PerfectZoneClass) return;

    // 이미 생성되어 있다면 제거
    if (PerfectZoneWidget)
    {
        PerfectZoneWidget->RemoveFromParent();
        PerfectZoneWidget = nullptr;
    }

    // 퍼펙트존 위젯 생성
    PerfectZoneWidget = CreateWidget<UUserWidget>(GetWorld(), PerfectZoneClass);
    if (!PerfectZoneWidget) return;

    // 스킬매니저 위젯의 캔버스패널에 추가
    if (UCanvasPanel* RootCanvas = Cast<UCanvasPanel>(GetRootWidget()))
    {
        RootCanvas->AddChild(PerfectZoneWidget);

        // 위치 세팅: 화면 중앙 하단에서 살짝 위
        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PerfectZoneWidget->Slot))
        {
            CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f)); // 화면 중앙 기준
            CanvasSlot->SetAlignment(FVector2D(0.5f, 1.0f)); // 위쪽 기준 조정
            CanvasSlot->SetPosition(FVector2D(0.f, -100.f)); // 중앙에서 살짝 위로
            CanvasSlot->SetSize(FVector2D(200.f, 200.f)); // 위젯 크기, 필요시 조정
        }
    }

    //// 여기서 FadeIn 애니메이션 실행
    //if (UWidgetAnimation* Anim = PerfectZoneWidget->FindAnimation(TEXT("FadeIn")))
    //{
    //    PlayAnimation(Anim);
    //}
}


