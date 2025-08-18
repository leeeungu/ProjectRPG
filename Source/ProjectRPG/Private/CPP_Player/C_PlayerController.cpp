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

void AC_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // EnhancedInputComponent�� ĳ����
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (TestAction)
        {
            // Q Ű�� ������ OnTestAction ȣ��
            EnhancedInput->BindAction(TestAction, ETriggerEvent::Triggered, this, &AC_PlayerController::OnTestAction);
        }
    }
}

void AC_PlayerController::OnTestAction(const FInputActionValue& Value)
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
        GetPawn()->SetActorLocation(res.ImpactPoint);
        AC_Player* player = Cast<AC_Player>(GetPawn());
        player->OnMoveToPos(res.ImpactPoint);
    }
    
}

AC_PlayerController::AC_PlayerController()
{
	bShowMouseCursor = true;
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(
		TEXT("/Game/RPG_Player/Input/PlayerInputMappingContexts.PlayerInputMappingContexts")
	);

	if (IMC.Succeeded())
	{
		InputMapping = IMC.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Test(
		TEXT("/Game/RPG_Player/Input/Actions/TestAction.TestAction")
	);
	if (IA_Test.Succeeded())
	{
		TestAction = IA_Test.Object;
	}
}

void AC_PlayerController::OnPossess(APawn* pawn)
{
    Super::OnPossess(pawn);
    // Enhanced Input Subsystem �������� (���� �÷��̾�)
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
