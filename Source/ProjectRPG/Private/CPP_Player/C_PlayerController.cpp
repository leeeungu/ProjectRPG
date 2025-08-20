// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "CPP_Player/C_Player.h"
#include "UObject/ConstructorHelpers.h"


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
        // �����ϰ� �α� ���
        if (Hit.GetActor())
        {
            UE_LOG(LogTemp, Warning, TEXT("Object Hit: %s at %s"),
                *Hit.GetActor()->GetName(),
                *Hit.ImpactPoint.ToString());
        }
        return;
    }

    // 2. Ʈ���̽� ä�� ��ŷ
    if (GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, Hit))
    {
        CachedMouseHit = Hit;
        CachedHitType = EMouseHitType::Ground;
        // �����ϰ� �α� ���
        UE_LOG(LogTemp, Warning, TEXT("Ground Hit at %s"), *Hit.ImpactPoint.ToString());

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
    AC_Player* player = Cast<AC_Player>(GetPawn());
    if (player && CachedHitType != EMouseHitType::None)
    {
        FVector TargetPoint = CachedMouseHit.ImpactPoint;
        player->CalRotateData(TargetPoint);//���콺��������ġ�� ����
        player->Period(); // �и���ų.
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
            Subsystem->AddMappingContext(InputMapping, 0);//�켱����0����
        }
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
