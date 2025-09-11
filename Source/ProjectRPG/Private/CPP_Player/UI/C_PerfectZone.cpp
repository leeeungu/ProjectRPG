// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/UI/C_PerfectZone.h"

void UC_PerfectZone::PlayProgressAnimation()
{
    if (Start)
    {
        PlayAnimation(Start);
    }
}

void UC_PerfectZone::StopProgressAnimation()
{
    if (Start && IsAnimationPlaying(Start))
    {
        StopAnimation(Start);
    }
}

void UC_PerfectZone::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
   
    if (!Bar || !BarBackup) return;

    // 현재 AnimImage의 RenderTransform 가져오기
    const FWidgetTransform& AnimTransform = Bar->RenderTransform;

    // X 스케일과 X 위치만 복사
    FWidgetTransform NewTransform = BarBackup->RenderTransform;

    NewTransform.Scale.X = AnimTransform.Scale.X;
    NewTransform.Translation.X = AnimTransform.Translation.X;

    // 적용
    BarBackup->SetRenderTransform(NewTransform);
    UE_LOG(LogTemp, Warning, TEXT("Ticking..."));
    //아 지금 t를누르면스케일이 변해서 쭉가다가 y누르면 초기화되잔 그럼 그초기화된값으로 또 틱이돌아서 setrender를 하니까 눌러도 소용이없는거구나.
}
