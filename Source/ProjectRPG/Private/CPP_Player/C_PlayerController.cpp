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

    // 1. 오브젝트 타입 피킹
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2));

    if (GetHitResultUnderCursorForObjects(ObjectTypes, false, Hit))
    {
        CachedMouseHit = Hit;
        CachedHitType = EMouseHitType::Object;
        // 간단하게 로그 출력
        if (Hit.GetActor())
        {
            UE_LOG(LogTemp, Warning, TEXT("Object Hit: %s at %s"),
                *Hit.GetActor()->GetName(),
                *Hit.ImpactPoint.ToString());
        }
        return;
    }

    // 2. 트레이스 채널 피킹
    if (GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, Hit))
    {
        CachedMouseHit = Hit;
        CachedHitType = EMouseHitType::Ground;
        // 간단하게 로그 출력
        UE_LOG(LogTemp, Warning, TEXT("Ground Hit at %s"), *Hit.ImpactPoint.ToString());

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
    AC_Player* player = Cast<AC_Player>(GetPawn());
    if (player && CachedHitType != EMouseHitType::None)
    {
        FVector TargetPoint = CachedMouseHit.ImpactPoint;
        player->CalRotateData(TargetPoint);//마우스포인터위치로 보간
        player->Period(); // 패링스킬.
    }
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
