// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "CPP_Player/C_Player.h"
#include "UObject/ConstructorHelpers.h"


void AC_PlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void AC_PlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    GetMousePos(IsOpenMousePointTrigger);
}

void AC_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // EnhancedInputComponent�� ĳ����
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (RightClick)
        {
            EnhancedInput->BindAction(RightClick, ETriggerEvent::Triggered, this, &AC_PlayerController::OnRightClickAction);
        }
    }
}

void AC_PlayerController::GetMousePos(bool IsOpenMousePoint)
{
    if (!IsOpenMousePoint) return;
    else if (IsOpenMousePoint)
    {
        FHitResult res;
        GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, res);
        UE_LOG(LogTemp, Warning, TEXT("OnTestAction Triggered!"));
        AC_Player* player = Cast<AC_Player>(GetPawn());
        player->SetMousePointDir(res.ImpactPoint);
    }
    

}

void AC_PlayerController::OnRightClickAction(const FInputActionValue& Value)
{
    FHitResult res;
    //������Ʈ Ÿ������ ��ŷ
    TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes{};
    objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2));
    if (GetHitResultUnderCursorForObjects(objectTypes, false, res))
    {
        AC_Player* player = Cast<AC_Player>(GetPawn());
        //player->OnBattle(res.GetActor()); (��Ʋ���� �ٷ� ����)
    }
    //Ʈ���̽� ä�η� ��ŷ
    else if (GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, res))
    {
        UE_LOG(LogTemp, Warning, TEXT("OnTestAction Triggered!"));
        AC_Player* player = Cast<AC_Player>(GetPawn());
        player->OnMoveToPosPlayer(res.ImpactPoint);
    }
    
}

AC_PlayerController::AC_PlayerController()
{
	bShowMouseCursor = true;//���콺ǥ��
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(
		TEXT("/Game/RPG_Player/Input/PlayerInputMappingContexts.PlayerInputMappingContexts")
	);

	if (IMC.Succeeded())
	{
		InputMapping = IMC.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_RightClick(
		TEXT("/Game/RPG_Player/Input/Actions/RighClick.RighClick")
	);
	if (IA_RightClick.Succeeded())
	{
        RightClick = IA_RightClick.Object;
	}
}

void AC_PlayerController::OnPossess(APawn* pawn)
{
    Super::OnPossess(pawn);
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (InputMapping)
        {
            // �켱���� 0���� ���� �߰�
            UE_LOG(LogTemp, Warning, TEXT("MappingContext Added!"));
            Subsystem->AddMappingContext(InputMapping, 0);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("MappingContext NOT Added!"));
        }
    }
}
