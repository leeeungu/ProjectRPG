// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/UI/C_PlayerStateMGR.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Player/C_Player.h"
#include "C_BaseCharacter.h"


void UC_PlayerStateMGR::NativeConstruct()
{
    Super::NativeConstruct();

    // 플레이어 캐릭터 캐스팅
    if (AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(this, 0))
    {
        if (AC_Player* MyPlayerBase = Cast<AC_Player>(PlayerActor))
        {
            // 델리게이트 바인딩
            MyPlayerBase->m_onDead.AddDynamic(this, &UC_PlayerStateMGR::OnPlayerDead);
            UE_LOG(LogTemp, Warning, TEXT("Dead_Binding"));
        }
    }
}

void UC_PlayerStateMGR::RestartPlayerGoing()
{
    SetVisibility(ESlateVisibility::Hidden);
    GetOwningPlayerPawn();
    AC_Player* myplayer = Cast<AC_Player>(GetOwningPlayerPawn());
    if (myplayer)
    {
        myplayer->restartPlayer();
    }

}

void UC_PlayerStateMGR::ExitGoing()
{
    OnEXITButtonClick.Broadcast();
}

void UC_PlayerStateMGR::OnPlayerDead()
{
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
