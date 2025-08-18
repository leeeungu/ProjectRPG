// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_Player.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

void AC_Player::CalMoveData()
{
	if (curPathPos >= pathList.Num())//�����Ұ��� �����? �׷� IsMoveȰ��ȭ���Ѽ� �ִϸ��̼� �۵���Ŵ ����.
	{
		//Cast<UCPP_AnimInstance>(skMesh->GetAnimInstance())->IsMove = false;
		return;
	}

	FVector pos = pathList[curPathPos++];
	pos.Z = GetActorLocation().Z;//Z�� ����(�ȸ��߸� ĳ������ default Z�� 0.5�������� ����� ���Ե�)
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
	//	//�������ƮŬ������ ��ӹ��� �������̽��� ����ȯ �Ұ��� �ϴ�.
	//	//IBattleTarget* t = Cast<IBattleTarget>(attackTarget); 
	//	if (attackTarget->GetClass()->ImplementsInterface(UBattleTarget::StaticClass()))
	//	{
	//		FVector pos = IBattleTarget::Execute_GetLocation(attackTarget);
	//		OnMoveToPos(pos);
	//	}
	//}->���� �������̽��� ������� �ȵ�.

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
	//��ã�� �н� ���ϱ�
	UNavigationPath* Path =
		UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), pos);
	if (Path != nullptr && Path->IsValid() && Path->PathPoints.Num() > 1)
	{
		//�н��� ���ߴ�.
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


