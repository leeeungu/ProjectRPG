// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_Player.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "CPP_Player/C_PlayerAnimInstance.h"
#include "CPP_Player/C_PlayerController.h"
#include "CPP_Player/C_InputQueueComponent.h"
#include "CPP_Player/C_SkillComponent.h"


void AC_Player::CalMoveData()
{
	if (curPathPos >= pathList.Num())//아무것도찍히지않으면 리스트의 원소개수는 1개임(현재위치)// 
	{
		Cast<UC_PlayerAnimInstance>(GetMesh()->GetAnimInstance())->IsMove = false;
		//가야할곳이없는데 혹시 remainDist,remainAngle이 남아있다면 초기화
		if (remainDist > 0.f || remainAngle > 0.f)
		{
			remainDist = 0.f;
			remainAngle = 0.f;
		}
		return;
	}
	FVector pos = pathList[curPathPos++];//(curPathPos는 제일먼저가야할곳, ++는 그다음path포인트임 = 다음위치정보를 담음
	pos.Z = GetActorLocation().Z;//Z축 맞춤(안맞추면 캐릭터의 default Z인 0.5지점부터 계산이 들어가게됨)
	moveDir = pos - GetActorLocation();
	remainDist = moveDir.Length();

	if (remainDist < stopDist)
	{
		remainDist = 0.f;
		return;
	}

	moveDir.Normalize();

	remainAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetActorForwardVector(), moveDir))); //회전각도
	rotDir = FVector::DotProduct(GetActorRightVector(), moveDir) > 0.f ? 1.f : -1.f;//회전방향
	Cast<UC_PlayerAnimInstance>(GetMesh()->GetAnimInstance())->IsMove = true;
	
}

void AC_Player::CalRotateData(const FVector& TargetPoint)
{
	// 현재 위치 → 목표 방향
	FVector Direction = TargetPoint - GetActorLocation();
	Direction.Z = 0.0f; // Pitch 무시
	Direction.Normalize();

	float TargetYaw = Direction.Rotation().Yaw;
	float CurrentYaw = GetActorRotation().Yaw;
	float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);
	float FinalYaw = CurrentYaw + DeltaYaw;
	TargetRotationQuat = FRotator(0.f, FinalYaw, 0.f).Quaternion();
	// 틱에서 회전 보간을 켜기 위한 플래그
	bRotate = true;
}

void AC_Player::Period()
{
	ClearMoveState();
	
	// 2. 대시 이동
	//float DashDistance = 300.0f;
	//FVector DashOffset = MousePointDir * DashDistance;

	//FHitResult Hit;
	//AddActorWorldOffset(DashOffset, true, &Hit);  // 충돌 체크 포함
}

AC_Player::AC_Player()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> res(TEXT("/Game/RPG_Hero_Character/Assets/Meshes/Adventurer/SK_Adventurer.SK_Adventurer"));
	if (res.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(res.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance>anim(TEXT("/Game/RPG_Player/ABP_Player.ABP_Player_C"));
	if (anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(anim.Class);
	}
	m_inputQueue = CreateDefaultSubobject<UC_InputQueueComponent>(TEXT("C_InputQueueComponent"));
	m_skillCom = CreateDefaultSubobject<UC_SkillComponent>(TEXT("C_SkillComponent"));

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	bUseControllerRotationYaw = false;

	m_springCom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRING"));
	m_springCom->SetupAttachment(RootComponent);
	m_springCom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	//m_springCom->SetRelativeLocation(FVector(-400.f, 0.f, 600.f));
	m_springCom->bDoCollisionTest = false; // 흔들림 방지
	m_springCom->bEnableCameraLag = false;
	m_springCom->bEnableCameraRotationLag = false;
	m_springCom->SetUsingAbsoluteRotation(true);
	m_springCom->bUsePawnControlRotation = false;
	m_springCom->TargetArmLength = 1500.0f;
	m_camCom = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	m_camCom->SetupAttachment(m_springCom);
	m_camCom->bUsePawnControlRotation = false;
}

void AC_Player::BeginPlay()
{
	Super::BeginPlay();
}

void AC_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (isBattle)
	//{
	//	//블루프린트클래스가 상속받은 인터페이스는 형변환 불가능 하다.
	//	//IBattleTarget* t = Cast<IBattleTarget>(attackTarget); 
	//	if (attackTarget->GetClass()->ImplementsInterface(UBattleTarget::StaticClass()))
	//	{
	//		FVector pos = IBattleTarget::Execute_GetLocation(attackTarget);
	//		OnMoveToPos(pos);
	//	}
	//}->아직 인터페이스및 몬스터통신 안됨.

	//moveDir.Normalize();
	//AddActorWorldOffset(moveDir.GetSafeNormal() * 200.0f * DeltaTime);

	if (remainDist > stopDist)
	{
		float delta = moveSpeed * DeltaTime;
		if (delta > remainDist) delta = remainDist;
		AddActorWorldOffset(moveDir * delta);
		remainDist -= delta;
		if (remainDist <= stopDist)
		{
			Cast<UC_PlayerAnimInstance>(GetMesh()->GetAnimInstance())->IsMove = false;
			CalMoveData();//
		}
	}
	else
	{
		Cast<UC_PlayerAnimInstance>(GetMesh()->GetAnimInstance())->IsMove = false;
	}

	if (remainAngle > 0.f)
	{
		float delta = 500.0f * DeltaTime;
		if (delta > remainAngle) delta = remainAngle;
		AddActorWorldRotation(FRotator(0.f, delta * rotDir, 0.f));
		remainAngle -= delta;
	}
	//마우스포인터위치로 보간회전
	if (bRotate)
	{
		ClearMoveState();
		FQuat CurrentQuat = GetActorQuat();
		float Dot = CurrentQuat | TargetRotationQuat;
		if (Dot < 0.f)
		{
			TargetRotationQuat = TargetRotationQuat * -1.f;
		}

		FQuat NewQuat = FQuat::Slerp(CurrentQuat, TargetRotationQuat, DeltaTime * RotateInterpSpeed);
		SetActorRotation(NewQuat);

		if (NewQuat.Equals(TargetRotationQuat, 0.05f))
		{
			bRotate = false;
			SetActorRotation(TargetRotationQuat);
		}
	}
	//period
	if (IsPeriod)
	{

	}
}

void AC_Player::OnMoveToPosPlayer(FVector pos)
{
	//길찾기 패스 구하기
	UNavigationPath* Path =
		UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), pos);
	if (Path != nullptr && Path->IsValid() && Path->PathPoints.Num() > 1)//네브메시볼륨에 찍혔고, 유효하고, 패스포인트가 2개이상인경우
	{
		pathList = Path->PathPoints;//리스트에 포인팅된 path정보를 담음(장애물이없다면 리스트에 2개가담김->[현재위치][찍은위치]
		curPathPos = 1;//if문을 넘겼다면 일단 이동해야하기떄문에 curPathPos를 1로 설정
	}
	CalMoveData();
}

//void AC_Player::SetMousePointDir(FVector pos)
//{
//	pos.Z = 0.0f;
//	MousePointDir = pos.GetSafeNormal();
//	UE_LOG(LogTemp, Warning, TEXT("SettinfComplete"));
//}

FVector AC_Player::GetMousePointDir()
{
	return this->MousePointDir;
}

void AC_Player::ClearMoveState()
{
	pathList.Empty();
	CalMoveData();
}


