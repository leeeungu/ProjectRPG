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

    // 1. ������Ʈ Ÿ�� ��ŷ
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2));

    if (GetHitResultUnderCursorForObjects(ObjectTypes, false, Hit))
    {
        CachedMouseHit = Hit;
        CachedHitType = EMouseHitType::Object;
        return;
    }

    // 2. Ʈ���̽� ä�� ��ŷ
    if (GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, Hit))
    {
        CachedMouseHit = Hit;
        CachedHitType = EMouseHitType::Ground;
        return;
    }

    // Hit�� ���� ��
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

    // EnhancedInputComponent�� ĳ����
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

//���콺 ������Ŭ��
void AC_PlayerController::OnRightClickAction(const FInputActionValue& Value)
{
    FHitResult CachedHit;
    EMouseHitType HitType;
    //Ÿ���� None�̸�false�� ������ �� �ƹ��͵� ��ŷ�Ȱ��̾���
    //true��°��� CachedHit�� ��ǥ���� ���õǰ�, HITŸ�Կ� Ÿ���� ����.
    if (!GetCachedMouseHit(CachedHit, HitType)) return;

    AC_Player* player = Cast<AC_Player>(GetPawn());
    if (!player || player->IsRotating()) return;//�÷��̾ �����������̸� �̵��Ұ�

    /*if (HitType == EMouseHitType::Object)
    {
        player->OnBattle(CachedHit.GetActor());
    }*/
    else if (HitType == EMouseHitType::Ground)
    {
        player->OnMoveToPosPlayer(CachedHit.ImpactPoint);
    }
}
//�����̽��� �Է�
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
//Q��ų �Է�
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
            Subsystem->AddMappingContext(InputMapping, 0);//�켱����0����
        }
    }
    if (pawn)
    {
        //�÷��̾��� ��ǲť������Ʈ �����ͼ� ����
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
//�׼Ǹ��콺 ����
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
