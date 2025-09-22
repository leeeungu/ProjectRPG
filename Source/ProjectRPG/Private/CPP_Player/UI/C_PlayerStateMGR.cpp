// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/UI/C_PlayerStateMGR.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Player/C_Player.h"


void UC_PlayerStateMGR::NativeConstruct()
{
    Super::NativeConstruct();

    // 플레이어 캐릭터 캐스팅
    if (AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(this, 0))
    {
        if (AC_Player* MyPlayer = Cast<AC_Player>(PlayerActor))
        {
            // 델리게이트 바인딩
            MyPlayer->m_onDead.AddDynamic(this, &UC_PlayerStateMGR::OnPlayerDead);
        }
    }
}

void UC_PlayerStateMGR::OnPlayerDead()
{
    if (!IsInViewport())
    {
        AddToViewport(5); // 뷰포트에 표시
        UE_LOG(LogTemp, Warning, TEXT("Restart"));
    }
}
