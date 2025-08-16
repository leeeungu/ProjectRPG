// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

void AC_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Enhanced Input Subsystem 가져오기 (로컬 플레이어)
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (InputMapping)
        {
            // 우선순위 0으로 매핑 추가
            Subsystem->AddMappingContext(InputMapping, 0);
        }
    }
}

void AC_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // EnhancedInputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (TestAction)
        {
            // Q 키가 눌리면 OnTestAction 호출
            EnhancedInput->BindAction(TestAction, ETriggerEvent::Triggered, this, &AC_PlayerController::OnTestAction);
        }
    }
}

void AC_PlayerController::OnTestAction(const FInputActionValue& Value)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Pressed Input Action");
}
