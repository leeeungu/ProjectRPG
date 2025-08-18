// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_Player.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

void AC_Player::CalMoveData()
{
	if (curPathPos >= pathList.Num())//가야할곳이 생겼다? 그럼 IsMove활성화시켜서 애니메이션 작동시킴 먼저.
	{
		//Cast<UCPP_AnimInstance>(skMesh->GetAnimInstance())->IsMove = false;
		return;
	}

	FVector pos = pathList[curPathPos++];
	pos.Z = GetActorLocation().Z;//Z축 맞춤(안맞추면 캐릭터의 default Z인 0.5지점부터 계산이 들어가게됨)
	moveDir = pos - GetActorLocation();
	remainDist = moveDir.Length();

	if (remainDist < stopDist)
	{
		remainDist = 0.f;
		return;
	}

	moveDir.Normalize();

	remainAngle =
		FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetActorForwardVector(), moveDir)));
	rotDir = FVector::DotProduct(GetActorRightVector(), moveDir) > 0.f ? 1.f : -1.f;
}

AC_Player::AC_Player()
{
	//skMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMESH"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> res(TEXT("/Game/RPG_Hero_Character/Assets/Meshes/Adventurer/SK_Adventurer.SK_Adventurer"));
	if (res.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(res.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	m_springCom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRING"));
	m_springCom->SetupAttachment(RootComponent);
	m_springCom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	m_springCom->SetRelativeLocation(FVector(-450.f, 0.f, 800.f));

	m_camCom = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	m_camCom->SetupAttachment(m_springCom);
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
			//Cast<UCPP_AnimInstance>(skMesh->GetAnimInstance())->IsMove = false;
			CalMoveData();
		}
	}
	else
	{
		//Cast<UCPP_AnimInstance>(skMesh->GetAnimInstance())->IsMove = false;
	}

	if (remainAngle > 0.f)
	{
		float delta = 360.0f * DeltaTime;
		if (delta > remainAngle) delta = remainAngle;
		AddActorWorldRotation(FRotator(0.f, delta * rotDir, 0.f));
		remainAngle -= delta;
	}
}

void AC_Player::OnMoveToPos(FVector pos)
{
	//길찾기 패스 구하기
	UNavigationPath* Path =
		UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), pos);
	if (Path != nullptr && Path->IsValid() && Path->PathPoints.Num() > 1)
	{
		//패스를 구했다.
		for (const FVector& Point : Path->PathPoints)
		{
			return;
		}
		pathList = Path->PathPoints;
		curPathPos = 1;
	}

	CalMoveData();
	//Cast<UCPP_AnimInstance>(skMesh->GetAnimInstance())->IsMove = true;
}


