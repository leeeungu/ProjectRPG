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
        CurrentTime = 0.0f;
        UpdateSecondText(CurrentTime);

        // 타이머 시작: 0.05초마다 호출
        GetWorld()->GetTimerManager().SetTimer(
            ProgressTimerHandle,
            this,
            &UC_PerfectZone::UpdateProgressTimer,
            0.05f,
            true
        );
        //IsTick = true;
        PlayAnimation(Start);
        UE_LOG(LogTemp, Warning, TEXT("Start"));
    }
}

void UC_PerfectZone::StopProgressAnimation()
{
    //IsTick = false;
    GetWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);
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
            
        }

        StopAnimation(Start);
        UE_LOG(LogTemp, Warning, TEXT("End"));
    }
    CurrentTime = 0.0f;
    UpdateSecondText(CurrentTime); // 정지 시 0초로 리셋
    
}

void UC_PerfectZone::UpdateSecondText(float TimeValue)
{
    if (Second)
    {
        // 소수 첫째 자리까지 문자열로 변환
        FString TimeString = FString::Printf(TEXT("%.1f"), TimeValue);
        Second->SetText(FText::FromString(TimeString));
    }
}

void UC_PerfectZone::NativeConstruct()
{
    Super::NativeConstruct();
    UpdateSecondText(0.0f);
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

void UC_PerfectZone::UpdateProgressTimer()
{
    CurrentTime += 0.05f;
    UpdateSecondText(CurrentTime);

    if (CurrentTime >= 0.8f)
    {
        // 자동으로 타이머 중지
        GetWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);
        CurrentTime = 0.0f;
    }
}

//void UC_PerfectZone::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
//{
//    Super::NativeTick(MyGeometry, InDeltaTime);
//    if (!IsTick)
//    {
//        CurrentTime = 0;
//        //UE_LOG(LogTemp, Warning, TEXT("TickENd"));
//        return;
//    }
//    CurrentTime += InDeltaTime;
//    UpdateSecondText(CurrentTime);
//    if (CurrentTime > 0.8f)
//    {
//        CurrentTime = 0;
//        IsTick = false;
//    }
//
//    //UE_LOG(LogTemp, Warning, TEXT("Ticking..."));
//}
