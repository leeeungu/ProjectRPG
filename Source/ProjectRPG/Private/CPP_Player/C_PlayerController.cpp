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

    // EnhancedInputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (RightClick)
        {
            EnhancedInput->BindAction(RightClick, ETriggerEvent::Triggered, this, &AC_PlayerController::OnRightClickAction);
        }
        if (SpaceBar)
        {
            EnhancedInput->BindAction(SpaceBar, ETriggerEvent::Started, this, &AC_PlayerController::OnSpaceBarAction);
        }
    }
}

void AC_PlayerController::GetMousePos(bool IsOpenMousePoint)//일단 한번열리면 바로 값전달하고 false로바꿈 (나중에 계속 열어야되는 때가오면 조건분기달아야함)
{
    if (!IsOpenMousePoint) return;
    else if (IsOpenMousePoint)
    {
        FHitResult res;
        GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, res);
        AC_Player* player = Cast<AC_Player>(GetPawn());
        player->SetMousePointDir(res.ImpactPoint);
        IsOpenMousePointTrigger = false;
    }
    

}
//마우스 오른쪽클릭
void AC_PlayerController::OnRightClickAction(const FInputActionValue& Value)
{
    FHitResult res;
    //오브젝트 타입으로 피킹
    TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes{};
    objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2));
    if (GetHitResultUnderCursorForObjects(objectTypes, false, res))
    {
        AC_Player* player = Cast<AC_Player>(GetPawn());
        //player->OnBattle(res.GetActor()); (배틀모드로 바로 진행)
    }
    //트레이스 채널로 피킹
    else if (GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, res))
    {
        AC_Player* player = Cast<AC_Player>(GetPawn());
        player->OnMoveToPosPlayer(res.ImpactPoint);
    }
    
}
//스페이스바 입력
void AC_PlayerController::OnSpaceBarAction(const FInputActionValue& Value)
{
    IsOpenMousePointTrigger = true;
    AC_Player* player = Cast<AC_Player>(GetPawn());
    player->Period();
}

AC_PlayerController::AC_PlayerController()
{
	bShowMouseCursor = true;//마우스표시
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
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_SpaceBar(
        TEXT("/Game/RPG_Player/Input/Actions/SpaceBar.SpaceBar")
    );
    if (IA_SpaceBar.Succeeded())
    {
        SpaceBar = IA_SpaceBar.Object;
    }
}

void AC_PlayerController::OnPossess(APawn* pawn)
{
    Super::OnPossess(pawn);
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (InputMapping)
        {
            Subsystem->AddMappingContext(InputMapping, 0);//우선순위0맵핑
        }
    }
}
