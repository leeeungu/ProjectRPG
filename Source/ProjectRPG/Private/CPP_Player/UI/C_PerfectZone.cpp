// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/UI/C_PerfectZone.h"
#include "CPP_Player/C_Player.h"

void UC_PerfectZone::PlayProgressAnimation()
{
    if (Start)
    {
        if (BarBackup)
        {
            // 에디터에서 잡아둔 초기 상태로 복원
            BarBackup->SetRenderTransform(InitialBackupTransform);
        }
        PlayAnimation(Start);
        UE_LOG(LogTemp, Warning, TEXT("Start"));
    }
}

void UC_PerfectZone::StopProgressAnimation()
{
    if (Start && IsAnimationPlaying(Start))
    {
        // Stop 직전에 현재 Bar Transform을 백업에 복사
        if (Bar && BarBackup)
        {
            const FWidgetTransform& AnimTransform = Bar->RenderTransform;

            FWidgetTransform NewTransform = BarBackup->RenderTransform;
            NewTransform.Scale.X = AnimTransform.Scale.X;
            NewTransform.Translation.X = AnimTransform.Translation.X;

            BarBackup->SetRenderTransform(NewTransform);
            UE_LOG(LogTemp, Warning, TEXT("End"));
        }

        StopAnimation(Start);
    }
    
}

void UC_PerfectZone::NativeConstruct()
{
    Super::NativeConstruct();

    if (APlayerController* myPC = GetOwningPlayer())
    {
        if (AC_Player* myPlayer = Cast<AC_Player>(myPC->GetPawn()))
        {
            myPlayer->OnChargeStart.AddDynamic(this, &UC_PerfectZone::PlayProgressAnimation);
            myPlayer->OnChargeEnd.AddDynamic(this, &UC_PerfectZone::StopProgressAnimation);
        }
    }
    if (BarBackup)
    {
        InitialBackupTransform = BarBackup->RenderTransform; // ← 에디터에서 세팅된 값 저장
    }
}

//void UC_PerfectZone::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
//{
//    Super::NativeTick(MyGeometry, InDeltaTime);
//   
//    if (!bTickActive) return; // 애니메이션 중일 때만 Tick 작동
//    if (!Bar || !BarBackup) return;
//
//    // 현재 AnimImage의 RenderTransform 가져오기
//    const FWidgetTransform& AnimTransform = Bar->RenderTransform;
//
//    // X 스케일과 X 위치만 복사
//    FWidgetTransform NewTransform = BarBackup->RenderTransform;
//    NewTransform.Scale.X = AnimTransform.Scale.X;
//    NewTransform.Translation.X = AnimTransform.Translation.X;
//
//    // 적용
//    BarBackup->SetRenderTransform(NewTransform);
//
//    UE_LOG(LogTemp, Warning, TEXT("Ticking..."));
//}
