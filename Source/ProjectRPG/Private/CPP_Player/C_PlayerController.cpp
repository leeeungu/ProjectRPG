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
#include "Quest/C_QuestManagerComponent.h"
#include "C_GameWindowManager.h"
#include "C_CurrencyComponent.h"
#include "Item/Component/C_EquipComponent.h"
#include "C_QuickSlotManagerComponent.h"
#include "C_InventoryComponent.h"
#include "C_ItemDataSubsystem.h"


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
        if (LeftClick)
        {
            EnhancedInput->BindAction(LeftClick, ETriggerEvent::Triggered, this, &AC_PlayerController::OnLeftClickAction);
        }
        if (SpaceBar)
        {
            EnhancedInput->BindAction(SpaceBar, ETriggerEvent::Started, this, &AC_PlayerController::OnSpaceBarAction);
        }
        if (Q_Key)
        {
            EnhancedInput->BindAction(Q_Key, ETriggerEvent::Started, this, &AC_PlayerController::OnQ_Action);
        }
        if (W_Key)
        {
            EnhancedInput->BindAction(W_Key, ETriggerEvent::Started, this, &AC_PlayerController::OnW_Action);
        }
        if (E_Key)
        {
            EnhancedInput->BindAction(E_Key, ETriggerEvent::Started, this, &AC_PlayerController::OnE_Action);
        }
        if (R_Key)
        {
            EnhancedInput->BindAction(R_Key, ETriggerEvent::Started, this, &AC_PlayerController::OnR_Action);
        }
        if (A_Key)
        {
            EnhancedInput->BindAction(A_Key, ETriggerEvent::Started, this, &AC_PlayerController::OnA_Action);
        }
        if (S_Key)
        {
            EnhancedInput->BindAction(S_Key, ETriggerEvent::Started, this, &AC_PlayerController::OnS_Action);
        }
        if (D_Key)
        {
            EnhancedInput->BindAction(D_Key, ETriggerEvent::Started, this, &AC_PlayerController::OnD_Action);
        }
        if (F_Key)
        {
            EnhancedInput->BindAction(F_Key, ETriggerEvent::Started, this, &AC_PlayerController::OnF_ActionStarted);
            EnhancedInput->BindAction(F_Key, ETriggerEvent::Triggered, this, &AC_PlayerController::OnF_ActionOngoing);
            EnhancedInput->BindAction(F_Key, ETriggerEvent::Canceled, this, &AC_PlayerController::OnF_ActionCanceld);
            EnhancedInput->BindAction(F_Key, ETriggerEvent::Completed, this, &AC_PlayerController::OnF_ActionCompleted);
        }
        /*if (Number1_Key)
        {
            EnhancedInput->BindAction(Number1_Key, ETriggerEvent::Started, this, &AC_PlayerController::OnNumber1_Action);
        }
        if (Number2_Key)
        {
            EnhancedInput->BindAction(Number2_Key, ETriggerEvent::Started, this, &AC_PlayerController::OnNumber2_Action);
        }*/

        if (m_pInteraction)
        {
            AC_Player* pPlayer = Cast< AC_Player>(AcknowledgedPawn);
            if (pPlayer)
            {
                EnhancedInput->BindAction(m_pInteraction, ETriggerEvent::Completed, pPlayer, &AC_Player::runInteraction);
            }
        }
      /*  if (m_pQuickSlot)
        {
            EnhancedInput->BindAction(m_pQuickSlot, ETriggerEvent::Completed, this, &AC_PlayerController::OnQuickSlot);
        }*/
        if(m_pQuickSlot1)
        {  
             EnhancedInput->BindAction(m_pQuickSlot1, ETriggerEvent::Completed, m_pQuickSlotManagerComponent, &UC_QuickSlotManagerComponent::useQuickSlotItemID, E_QuickSlotType::E_QuickSlot1);
        }
        if (m_pQuickSlot2)
        {  
             EnhancedInput->BindAction(m_pQuickSlot2, ETriggerEvent::Completed, m_pQuickSlotManagerComponent, &UC_QuickSlotManagerComponent::useQuickSlotItemID, E_QuickSlotType::E_QuickSlot2);
        }
        if (m_pQuickSlot3)
        {  
             EnhancedInput->BindAction(m_pQuickSlot3, ETriggerEvent::Completed, m_pQuickSlotManagerComponent, &UC_QuickSlotManagerComponent::useQuickSlotItemID, E_QuickSlotType::E_QuickSlot3);
        }
        if (m_pQuickSlot4)
        {  
             EnhancedInput->BindAction(m_pQuickSlot4, ETriggerEvent::Completed, m_pQuickSlotManagerComponent, &UC_QuickSlotManagerComponent::useQuickSlotItemID, E_QuickSlotType::E_QuickSlot4);
        }
        if (m_pInventoryWidget)
        {
            EnhancedInput->BindAction(m_pInventoryWidget, ETriggerEvent::Completed, m_pGameWindowManager, &UC_GameWindowManager::toggleWindow, E_WindowType::E_Inventory);
        }
        if (m_pPlayerInfoWidget)
        {
            EnhancedInput->BindAction(m_pPlayerInfoWidget, ETriggerEvent::Completed, m_pGameWindowManager, &UC_GameWindowManager::toggleWindow, E_WindowType::E_PlayerInfo);
        }
        if (m_pQuestWidget)
        {
            EnhancedInput->BindAction(m_pQuestWidget, ETriggerEvent::Completed, m_pGameWindowManager, &UC_GameWindowManager::toggleWindow, E_WindowType::E_QuestWindow);
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
void AC_PlayerController::OnLeftClickAction(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "PA_01";
    NewInputData.InputType = EInputType::PlainAttack;
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
        UE_LOG(LogTemp, Warning, TEXT("Attack"));
    }
}
//스페이스바 입력
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
//스킬 입력
void AC_PlayerController::OnQ_Action(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "S_01";
    NewInputData.InputType = EInputType::Skill; 
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}
void AC_PlayerController::OnW_Action(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "S_02";
    NewInputData.InputType = EInputType::Skill;
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}
void AC_PlayerController::OnE_Action(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "S_03";
    NewInputData.InputType = EInputType::Skill;
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}
void AC_PlayerController::OnR_Action(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "S_04";
    NewInputData.InputType = EInputType::Skill;
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}
void AC_PlayerController::OnA_Action(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "S_05";
    NewInputData.InputType = EInputType::Skill;
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}
void AC_PlayerController::OnS_Action(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "S_06";
    NewInputData.InputType = EInputType::Skill;
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}
void AC_PlayerController::OnD_Action(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "S_07";
    NewInputData.InputType = EInputType::Skill;
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}
//R스킬(차징스킬)
void AC_PlayerController::OnF_ActionStarted(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "ChargingStartSkill";
    NewInputData.InputType = EInputType::ChargeSkill;
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    UE_LOG(LogTemp, Warning, TEXT("[Input] F Skill Triggered: Started"));
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput(NewInputData);
    }
}
void AC_PlayerController::OnF_ActionOngoing(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "ChargingHoldingSkill";
    NewInputData.InputType = EInputType::ChargeSkill;
    NewInputData.InputStateType = EInputStateType::Held;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput_Charging(NewInputData);
    }
}
void AC_PlayerController::OnF_ActionCanceld(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "ChargingEndCancelSkill";
    NewInputData.InputType = EInputType::ChargeSkill;
    NewInputData.InputStateType = EInputStateType::Released;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput_Charging(NewInputData);
    }
}
void AC_PlayerController::OnF_ActionCompleted(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "ChargingEndCompleteSkill";
    NewInputData.InputType = EInputType::ChargeSkill;
    NewInputData.InputStateType = EInputStateType::Released;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
    if (InputQueueSystem)
    {
        InputQueueSystem->PushInput_Charging(NewInputData);
    }
}
//아이템(즉발)->바로함수이벤트처리
void AC_PlayerController::OnNumber1_Action(const FInputActionValue& Value)
{
    //ITem useFunc();
}
//아이템(애니메이션)->인풋큐add ex회오리수류탄
void AC_PlayerController::OnNumber2_Action(const FInputActionValue& Value)
{
    FInputActionData NewInputData;
    NewInputData.ActionName = "item1";
    NewInputData.InputType = EInputType::AnimItem;
    NewInputData.InputStateType = EInputStateType::Pressed;
    NewInputData.TargetPoint = CachedMouseHit.ImpactPoint;
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

        AC_Player* pPlayer = Cast< AC_Player>(pawn);
        UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
        if (pPlayer)
        {
            EnhancedInput->BindAction(m_pInteraction, ETriggerEvent::Completed, pPlayer, &AC_Player::runInteraction);
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
    if (IMC.Succeeded()) InputMapping = IMC.Object;
    //마우스클릭
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_RightClick(TEXT("/Game/RPG_Player/Input/Actions/RighClick.RighClick"));
    if (IA_RightClick.Succeeded()) RightClick = IA_RightClick.Object;
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_LeftClick(TEXT("/Game/RPG_Player/Input/Actions/LeftClick.LeftClick"));
    if (IA_LeftClick.Succeeded()) LeftClick = IA_LeftClick.Object;
    //스페이스바(패링)
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_SpaceBar(TEXT("/Game/RPG_Player/Input/Actions/SpaceBar.SpaceBar"));
    if (IA_SpaceBar.Succeeded()) SpaceBar = IA_SpaceBar.Object;
    //스킬
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_QAction(TEXT("/Game/RPG_Player/Input/Actions/Q_Action.Q_Action"));
    if (IA_QAction.Succeeded()) Q_Key = IA_QAction.Object;
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_WAction(TEXT("/Game/RPG_Player/Input/Actions/W_Action.W_Action"));
    if (IA_QAction.Succeeded()) W_Key = IA_WAction.Object;
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_EAction(TEXT("/Game/RPG_Player/Input/Actions/E_Action.E_Action"));
    if (IA_QAction.Succeeded()) E_Key = IA_EAction.Object;
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_RAction(TEXT("/Game/RPG_Player/Input/Actions/R_Action.R_Action"));
    if (IA_QAction.Succeeded()) R_Key = IA_RAction.Object;
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_AAction(TEXT("/Game/RPG_Player/Input/Actions/A_Action.A_Action"));
    if (IA_QAction.Succeeded()) A_Key = IA_AAction.Object;
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_SAction(TEXT("/Game/RPG_Player/Input/Actions/S_Action.S_Action"));
    if (IA_QAction.Succeeded()) S_Key = IA_SAction.Object;
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_DAction(TEXT("/Game/RPG_Player/Input/Actions/D_Action.D_Action"));
    if (IA_QAction.Succeeded()) D_Key = IA_DAction.Object;
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_FAction(TEXT("/Game/RPG_Player/Input/Actions/F_Action.F_Action"));
    if (IA_FAction.Succeeded()) F_Key = IA_FAction.Object;
    //아이템
    /*static ConstructorHelpers::FObjectFinder<UInputAction> IA_Number1Action(TEXT("/Game/RPG_Player/Input/Actions/Number1_Action.Number1_Action"));
    if (IA_Number1Action.Succeeded()) Number1_Key = IA_Number1Action.Object;
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_Number2Action(TEXT("/Game/RPG_Player/Input/Actions/Number2_Acrion.Number2_Acrion"));
    if (IA_Number2Action.Succeeded()) Number2_Key = IA_Number2Action.Object;*/
    
    m_pInventoryComponent = CreateDefaultSubobject<UC_InventoryComponent>(TEXT("InventoryComponent"));
    m_pCurrencyComponent = CreateDefaultSubobject<UC_CurrencyComponent>(TEXT("CurrencyComponent"));
    m_pGameWindowManager = CreateDefaultSubobject<UC_GameWindowManager>(TEXT("GameWindowManager"));
    m_pQuickSlotManagerComponent = CreateDefaultSubobject<UC_QuickSlotManagerComponent>(TEXT("QuickSlotManagerComponent"));
    m_pQuestManagerComponent2 = CreateDefaultSubobject<UC_QuestManagerComponent>(TEXT("QuestManagerComponent"));
    m_pEquipComponent = CreateDefaultSubobject<UC_EquipComponent>(TEXT("EquipComponent"));

}
