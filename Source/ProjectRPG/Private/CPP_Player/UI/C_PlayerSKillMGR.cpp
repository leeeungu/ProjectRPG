// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/UI/C_PlayerSKillMGR.h"
#include "CPP_Player/UI/C_PerfectZone.h"
#include "CPP_Player/UI/C_PerfectZoneResult.h"
#include "CPP_Player/C_Player.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"



void UC_PlayerSKillMGR::ShowPerfectZone()
{
    if (!PerfectZoneWidget || !PerfectZoneWidget->Start) return;
    PerfectZoneWidget->SetVisibility(ESlateVisibility::Visible);
    PerfectZoneWidget->PlayAnimation(PerfectZoneWidget->Show);
}

void UC_PlayerSKillMGR::InitPerfectZone()
{
    if (!PerfectZoneClass) return;

    if (!PerfectZoneWidget)
    {
        // 위젯 생성
        UUserWidget* RawWidget = CreateWidget<UUserWidget>(GetWorld(), PerfectZoneClass);
        PerfectZoneWidget = Cast<UC_PerfectZone>(RawWidget);

        if (!PerfectZoneWidget) return;

        // 루트 캔버스에 추가
        if (UCanvasPanel* RootCanvas = Cast<UCanvasPanel>(GetRootWidget()))
        {
            RootCanvas->AddChild(PerfectZoneWidget);

            if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PerfectZoneWidget->Slot))
            {
                CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
                CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
                CanvasSlot->SetPosition(FVector2D(0.f, 200.f));
                CanvasSlot->SetSize(FVector2D(200.f, 200.f));
            }

            PerfectZoneWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UC_PlayerSKillMGR::HiddenPerfectZone()
{
    PerfectZoneWidget->PlayAnimation(PerfectZoneWidget->NotShow);
    PerfectZoneWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UC_PlayerSKillMGR::InitResultWidgets()
{
    if (SuccessWidgetClass && !SuccessWidgetInstance)
    {
        SuccessWidgetInstance = CreateWidget<UC_PerfectZoneResult>(GetWorld(), SuccessWidgetClass);
        if (SuccessWidgetInstance)
        {
            // 루트 캔버스에 추가
            if (UCanvasPanel* RootCanvas = Cast<UCanvasPanel>(GetRootWidget()))
            {
                RootCanvas->AddChild(SuccessWidgetInstance);

                if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(SuccessWidgetInstance->Slot))
                {
                    CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
                    CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
                    CanvasSlot->SetPosition(FVector2D(0.f, 170.f));
                    CanvasSlot->SetSize(FVector2D(200.f, 200.f));
                }
                SuccessWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
            }
        }
    }

    if (FailWidgetClass && !FailWidgetInstance)
    {
        FailWidgetInstance = CreateWidget<UC_PerfectZoneResult>(GetWorld(), FailWidgetClass);
        if (FailWidgetInstance)
        {
            // 루트 캔버스에 추가
            if (UCanvasPanel* RootCanvas = Cast<UCanvasPanel>(GetRootWidget()))
            {
                RootCanvas->AddChild(FailWidgetInstance);

                if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(FailWidgetInstance->Slot))
                {
                    CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
                    CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
                    CanvasSlot->SetPosition(FVector2D(0.f, 170.f));
                    CanvasSlot->SetSize(FVector2D(200.f, 200.f));
                }
                FailWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
            }
        }
    }
}

void UC_PlayerSKillMGR::ShowResult(bool result)
{
    if (!SuccessWidgetInstance && !FailWidgetInstance) return;
    if (result)//성공
    {
        SuccessWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        SuccessWidgetInstance->Show();
    }
    if (!result)//실패
    {
        FailWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        FailWidgetInstance->Show();
    }
}

void UC_PlayerSKillMGR::NativeConstruct()
{
    Super::NativeConstruct();
    InitPerfectZone();
    InitResultWidgets();
    if (APlayerController* myPC = GetOwningPlayer())
    {
        if (AC_Player* myPlayer = Cast<AC_Player>(myPC->GetPawn()))
        {
            myPlayer->OnResultOpen.AddDynamic(this, &UC_PlayerSKillMGR::ShowResult);
        }
    }
}




