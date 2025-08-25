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
        if (Q_Key)
        {
            EnhancedInput->BindAction(Q_Key, ETriggerEvent::Started, this, &AC_PlayerController::OnQ_Action);
        }
        if (R_Key)
        {
            EnhancedInput->BindAction(R_Key, ETriggerEvent::Started, this, &AC_PlayerController::OnR_ActionStarted);
            EnhancedInput->BindAction(R_Key, ETriggerEvent::Triggered, this, &AC_PlayerController::OnR_ActionOngoing);
            EnhancedInput->BindAction(R_Key, ETriggerEvent::Canceled, this, &AC_PlayerController::OnR_ActionCanceld);
            EnhancedInput->BindAction(R_Key, ETriggerEvent::Completed, this, &AC_PlayerController::OnR_ActionCompleted);\

            if (R_Key->Triggers.Num() >= 1 && Cast< UInputTriggerHold>(R_Key->Triggers[0].Get()))//��¡ ��ų ����ġ �׽�Ʈ
                UE_LOG(C_PlayerController, Error, TEXT("%f"), Cast< UInputTriggerHold>(R_Key->Triggers[0].Get())->HoldTimeThreshold);
        }
        if (Number1_Key)
        {
            EnhancedInput->BindAction(Number1_Key, ETriggerEvent::Started, this, &AC_PlayerController::OnNumber1_Action);
        }
        if (Number2_Key)
        {
            EnhancedInput->BindAction(Number2_Key, ETriggerEvent::Started, this, &AC_PlayerController::OnNumber2_Action);
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
    NewInputData.ActionName = "Period";
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
    NewInputData.ActionName = "Skill_01";
    NewInputData.InputType = EInputType::Skill; 
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}
//R��ų(��¡��ų)
void AC_PlayerController::OnR_ActionStarted(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "ChargingSkill";
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
    NewInputData.ActionName = "ChargingSkill";
    NewInputData.InputType = EInputType::ChargeSkill;
    NewInputData.InputStateType = EInputStateType::Held;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    UE_LOG(LogTemp, Warning, TEXT("[Input] R Skill Triggered: Ongoing"));
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}
void AC_PlayerController::OnR_ActionCanceld(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "ChargingSkill";
    NewInputData.InputType = EInputType::ChargeSkill;
    NewInputData.InputStateType = EInputStateType::Released;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    UE_LOG(LogTemp, Warning, TEXT("[Input] R Skill Triggered: Completed"));//������ canceld���� complete�� �����ϰ�ó��
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}
void AC_PlayerController::OnR_ActionCompleted(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "ChargingSkill";
    NewInputData.InputType = EInputType::ChargeSkill;
    NewInputData.InputStateType = EInputStateType::Released;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    UE_LOG(LogTemp, Warning, TEXT("[Input] R Skill Triggered: Completed"));
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}
//������(���)->�ٷ��Լ��̺�Ʈó��
void AC_PlayerController::OnNumber1_Action(const FInputActionValue& Value)
{
    //ITem useFunc();
}
//������(�ִϸ��̼�)->��ǲťadd exȸ��������ź
void AC_PlayerController::OnNumber2_Action(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "item1";
    NewInputData.InputType = EInputType::AnimItem;
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    UE_LOG(LogTemp, Warning, TEXT("[Input] number2 Skill On"));
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
    //���콺Ŭ��
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_RightClick(
        TEXT("/Game/RPG_Player/Input/Actions/RighClick.RighClick")
    );
    if (IA_RightClick.Succeeded())
    {
        RightClick = IA_RightClick.Object;
    }
    //�����̽���(�и�)
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_SpaceBar(
        TEXT("/Game/RPG_Player/Input/Actions/SpaceBar.SpaceBar")
    );
    if (IA_SpaceBar.Succeeded())
    {
        SpaceBar = IA_SpaceBar.Object;
    }
    //��ų
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_QAction(
        TEXT("/Game/RPG_Player/Input/Actions/Q_Action.Q_Action")
    );
    if (IA_QAction.Succeeded())
    {
        Q_Key = IA_QAction.Object;
    }
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_RAction(
        TEXT("/Game/RPG_Player/Input/Actions/R_Action.R_Action")
    );
    if (IA_RAction.Succeeded())
    {
        R_Key = IA_RAction.Object;
    }
    //������
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_Number1Action(
        TEXT("/Game/RPG_Player/Input/Actions/Number1_Action.Number1_Action")
    );
    if (IA_Number1Action.Succeeded())
    {
        Number1_Key = IA_Number1Action.Object;
    }
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_Number2Action(
        TEXT("/Game/RPG_Player/Input/Actions/Number2_Acrion.Number2_Acrion")
    );
    if (IA_Number2Action.Succeeded())
    {
        Number2_Key = IA_Number2Action.Object;
    }

}
