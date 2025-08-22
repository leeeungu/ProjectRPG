// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "CPP_Player/C_Player.h"
#include "UObject/ConstructorHelpers.h"
#include "CPP_Player/C_InputQueueComponent.h"
#include "CPP_Player/S_InputActionData.h"

DEFINE_LOG_CATEGORY_STATIC(C_PlayerController, Log, All);

void AC_PlayerController::UpdateMouseHit()
{
    FHitResult Hit;
    CachedHitType = EMouseHitType::None;

    // 1. 오브젝트 타입 피킹
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2));

    if (GetHitResultUnderCursorForObjects(ObjectTypes, false, Hit))
    {
        CachedMouseHit = Hit;
        CachedHitType = EMouseHitType::Object;
        return;
    }

    // 2. 트레이스 채널 피킹
    if (GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, Hit))
    {
        CachedMouseHit = Hit;
        CachedHitType = EMouseHitType::Ground;
        return;
    }

    // Hit이 없을 때
    CachedMouseHit = FHitResult();
}

void AC_PlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void AC_PlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateMouseHit();
    //GetMousePos(IsOpenMousePointTrigger);
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
        if (Q_Skill)
        {
            EnhancedInput->BindAction(Q_Skill, ETriggerEvent::Started, this, &AC_PlayerController::OnQ_Action);
        }
        if (R_Skill)
        {
            EnhancedInput->BindAction(R_Skill, ETriggerEvent::Started, this, &AC_PlayerController::OnR_ActionStarted);
            EnhancedInput->BindAction(R_Skill, ETriggerEvent::Triggered, this, &AC_PlayerController::OnR_ActionOngoing);
            EnhancedInput->BindAction(R_Skill, ETriggerEvent::Canceled, this, &AC_PlayerController::OnR_ActionCompleted);\

            if (R_Skill->Triggers.Num() >= 1 && Cast< UInputTriggerHold>(R_Skill->Triggers[0].Get()))
                UE_LOG(C_PlayerController, Error, TEXT("%f"), Cast< UInputTriggerHold>(R_Skill->Triggers[0].Get())->HoldTimeThreshold);
        }
    }
}

//마우스 오른쪽클릭
void AC_PlayerController::OnRightClickAction(const FInputActionValue& Value)
{
    FHitResult CachedHit;
    EMouseHitType HitType;
    //타입이 None이면false를 리턴함 즉 아무것도 피킹된것이없음
    //true라는것은 CachedHit에 좌표값이 세팅되고, HIT타입에 타입이 들어옴.
    if (!GetCachedMouseHit(CachedHit, HitType)) return;

    AC_Player* player = Cast<AC_Player>(GetPawn());
    if (!player || player->IsRotating()) return;//플레이어가 로테이팅중이면 이동불가

    /*if (HitType == EMouseHitType::Object)
    {
        player->OnBattle(CachedHit.GetActor());
    }*/
    else if (HitType == EMouseHitType::Ground)
    {
        player->OnMoveToPosPlayer(CachedHit.ImpactPoint);
    }
}
//스페이스바 입력
void AC_PlayerController::OnSpaceBarAction(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionIndex = 1;
    NewInputData.InputType = EInputType::Period;
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}
//Q스킬 입력
void AC_PlayerController::OnQ_Action(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionIndex = 2;
    NewInputData.InputType = EInputType::Skill; 
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}

void AC_PlayerController::OnR_ActionStarted(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionIndex = 5;
    NewInputData.InputType = EInputType::ChargeSkill;
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    UE_LOG(LogTemp, Warning, TEXT("[Input] R Skill Triggered: Started"));
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}

void AC_PlayerController::OnR_ActionOngoing(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionIndex = 5;
    NewInputData.InputType = EInputType::ChargeSkill;
    NewInputData.InputStateType = EInputStateType::Held;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    UE_LOG(LogTemp, Warning, TEXT("[Input] R Skill Triggered: Ongoing"));
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}

void AC_PlayerController::OnR_ActionCompleted(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionIndex = 5;
    NewInputData.InputType = EInputType::ChargeSkill;
    NewInputData.InputStateType = EInputStateType::Released;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    UE_LOG(LogTemp, Warning, TEXT("[Input] R Skill Triggered: Completed"));
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
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
    if (pawn)
    {
        //플레이어의 인풋큐컴포넌트 가져와서 세팅
        InputQueueSystem = pawn->FindComponentByClass<UC_InputQueueComponent>();
    }
}

bool AC_PlayerController::GetCachedMouseHit(FHitResult& OutHit, EMouseHitType& OutType) const
{
    if (CachedHitType != EMouseHitType::None)
    {
        OutHit = CachedMouseHit;
        OutType = CachedHitType;
        return true;
    }
    return false;
}
//액션마우스 세팅
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
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_QAction(
        TEXT("/Game/RPG_Player/Input/Actions/Q_Action.Q_Action")
    );
    if (IA_QAction.Succeeded())
    {
        Q_Skill = IA_QAction.Object;
    }
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_RAction(
        TEXT("/Game/RPG_Player/Input/Actions/R_Action.R_Action")
    );
    if (IA_RAction.Succeeded())
    {
        R_Skill = IA_RAction.Object;
    }
}
