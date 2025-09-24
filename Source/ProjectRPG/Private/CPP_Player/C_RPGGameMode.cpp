// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_RPGGameMode.h"
#include "CPP_Player/C_Player.h"
#include "CPP_Player/C_PlayerController.h"
#include "GamePlay/C_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"


AC_RPGGameMode::AC_RPGGameMode()
{
	//DefaultPawnClass = AC_Player::StaticClass();
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/RPG_Player/BP_Player.BP_Player_C"));
    if (PlayerPawnBPClass.Class != nullptr)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
	PlayerControllerClass = AC_PlayerController::StaticClass();
}

void AC_RPGGameMode::BeginPlay()
{
    Super::BeginPlay();

    // GameInstance 가져오기
    UC_GameInstance* GI = Cast<UC_GameInstance>(GetGameInstance());
    if (!GI)
    {
        return;
    }

    // Restart_Point 태그를 가진 액터들 찾기
    TArray<AActor*> RestartPoints;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Restart_Point"), RestartPoints);

    if (RestartPoints.Num() > 0)
    {
        FVector RestartLocation = RestartPoints[0]->GetActorLocation();
        GI->SetRestartLocation(RestartLocation);
    }
    
}
