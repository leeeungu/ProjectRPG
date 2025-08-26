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
#include "CPP_Player/S_InputActionData.h"


void AC_Player::HandleChangeRunningState()
{
	RunningState = ERunningSystemState::Idle;
}

void AC_Player::CalMoveData()
{
	if (curPathPos >= pathList.Num())//�ƹ��͵������������� ����Ʈ�� ���Ұ����� 1����(������ġ)// 
	{
		Cast<UC_PlayerAnimInstance>(GetMesh()->GetAnimInstance())->IsMove = false;
		//�����Ұ��̾��µ� Ȥ�� remainDist,remainAngle�� �����ִٸ� �ʱ�ȭ
		if (remainDist > 0.f || remainAngle > 0.f)
		{
			remainDist = 0.f;
			remainAngle = 0.f;
		}
		return;
	}
	FVector pos = pathList[curPathPos++];//(curPathPos�� ���ϸ��������Ұ�, ++�� �״���path����Ʈ�� = ������ġ������ ����
	pos.Z = GetActorLocation().Z;//Z�� ����(�ȸ��߸� ĳ������ default Z�� 0.5�������� ����� ���Ե�)
	moveDir = pos - GetActorLocation();
	remainDist = moveDir.Length();

	if (remainDist < stopDist)
	{
		remainDist = 0.f;
		return;
	}

	moveDir.Normalize();

	remainAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetActorForwardVector(), moveDir))); //ȸ������
	rotDir = FVector::DotProduct(GetActorRightVector(), moveDir) > 0.f ? 1.f : -1.f;//ȸ������
	Cast<UC_PlayerAnimInstance>(GetMesh()->GetAnimInstance())->IsMove = true;
	
}

void AC_Player::CalRotateData(const FVector& TargetPoint)
{
	// ���� ��ġ �� ��ǥ ����
	FVector Direction = TargetPoint - GetActorLocation();
	Direction.Z = 0.0f; // Pitch ����
	Direction.Normalize();

	float TargetYaw = Direction.Rotation().Yaw;
	float CurrentYaw = GetActorRotation().Yaw;
	float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);
	float FinalYaw = CurrentYaw + DeltaYaw;
	TargetRotationQuat = FRotator(0.f, FinalYaw, 0.f).Quaternion();
	// ƽ���� ȸ�� ������ �ѱ� ���� �÷���
	bRotate = true;
}

//���η��� �Ŵ���
void AC_Player::RunningSystemManager()
{
	if (RunningState == ERunningSystemState::Idle)
	{
		FInputActionData CurrentInputData{};//����ִ� �ʱⰪ.
		if (m_inputQueue->GetLastInputData(CurrentInputData))//��ǲ�� ���� ���´ٸ�
		{
			m_inputQueue->ClearQueueList();//�׳� �������ε����� �����°ű⶧���� ������ �ٽ� ť���� ������ �׷��⋚���� ���������� ����Ʈ�������� ������ �ڵ����� ������������.
			switch (CurrentInputData.InputType)
			{
			case EInputType::Skill:
				RunningState = ERunningSystemState::Busy;
				CalRotateData(CurrentInputData.TargetPoint);
				m_skillCom->UsingSkill(CurrentInputData.ActionName);//��Ʈ�ѷ����� ������� name�� ����ü�� ��ųname�� ���ƾ���.
				break;
			case EInputType::AnimItem:
				RunningState = ERunningSystemState::Busy;
				CalRotateData(CurrentInputData.TargetPoint);
				//ExecuteSkill(CurrentInputData); ->�����Լ�
				break;
			case EInputType::Period:
				RunningState = ERunningSystemState::Busy;
				CalRotateData(CurrentInputData.TargetPoint);
				//ExecuteSkill(CurrentInputData); ->�����Լ�
				break;

			case EInputType::Item:
				// ��� �������� Busy/Charging ���¿��� ��� �����ϵ��� ���� ó��
				//UseItem(CurrentInputData);
				break;

			case EInputType::ChargeSkill:
				RunningState = ERunningSystemState::Charging;
				//StartChargeSkill(CurrentInputData);->��¡�����Լ�(�ð�����ʿ�, ��Ÿ��Ȧ���ʿ�)
				break;

			default:
				break;
			}
		}
	}
	else if (RunningState == ERunningSystemState::Charging)//�̹�idle���� ��¡��ŸƮ�� ���º���Ǽ��Ѿ��
	{
		// ��¡ ��ų �Է¸� ��� -> ������ ����
		FInputActionData ChargeInput{};
		if (m_inputQueue->GetLastInputData(ChargeInput))//��� �ش���¡��ų�������Է��̵��ð���(�ε�����ȣ��, Trigged�̵�)
		{
			//���� ��ǲŸ���� charing������ �̹� �������� ��ŸƮ�� idle���¿��� �ν��ϰ� �Ѿ�Ա⿡ ���� ������ (Held,Canceld,Completed)
			switch (ChargeInput.InputStateType)
			{
			case EInputStateType::Held:
				//UpdateChargeSkill(ChargeInput); // Triggered �ܰ� ó��,�������ִ���
				break;
			case EInputStateType::Released://ĵ���� �Ϸ��϶� ��� Released�� ���õ�
				//ImpactChargeSkill(ChargeInput); // �̶� ��Ÿ�ֿ��� �ٽ� Idle���·� �ٲ������.
				break;
			}
		}
	}
	else if (RunningState == ERunningSystemState::Busy)
	{
		return;//�Ϲ� �������� ó��(���࿡ ��ų������̳� ��¡��ų����߿� ���� �Է��� �޾ƾ��Ѵٸ� �׳� �ٷ��̺�Ʈ�� ó��(ť�� add X)
	}

}

void AC_Player::Period()
{
	ClearMoveState();
	
	// 2. ��� �̵�
	//float DashDistance = 300.0f;
	//FVector DashOffset = MousePointDir * DashDistance;

	//FHitResult Hit;
	//AddActorWorldOffset(DashOffset, true, &Hit);  // �浹 üũ ����
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
	m_springCom->bDoCollisionTest = false; // ��鸲 ����
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

	if (USkeletalMeshComponent* myMesh = GetMesh())
	{
		UC_PlayerAnimInstance* myAnimInstance = Cast<UC_PlayerAnimInstance>(myMesh->GetAnimInstance());
		if (myAnimInstance)
		{
			// ���⼭ ��������Ʈ ���ε�
			myAnimInstance->ChangeRunningState.RemoveAll(this);//������ġ(������� ĳ���Ͱ� �װ� �ٽû�Ƴ���.
			myAnimInstance->ChangeRunningState.AddUObject(this, &AC_Player::HandleChangeRunningState);
			//���� ��Ƽ���̹߻��� �ִ��ν��Ͻ����� ��ε�ĳ��Ʈ�� �÷��̾�� ����
			//�÷��̾�� ���ε��� 'HandleChangeRunningState' ����
		}
	}

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
	//���콺��������ġ�� ����ȸ��(���� ��ų������ �ش���ġ�� �����������۾��ҋ��� ����/
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
	////period
	//if (IsPeriod)
	//{

	//}
	RunningSystemManager();

}

void AC_Player::OnMoveToPosPlayer(FVector pos)
{
	//��ã�� �н� ���ϱ�
	UNavigationPath* Path =
		UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), pos);
	if (Path != nullptr && Path->IsValid() && Path->PathPoints.Num() > 1)//�׺�޽ú����� ������, ��ȿ�ϰ�, �н�����Ʈ�� 2���̻��ΰ��
	{
		pathList = Path->PathPoints;//����Ʈ�� �����õ� path������ ����(��ֹ��̾��ٸ� ����Ʈ�� 2�������->[������ġ][������ġ]
		curPathPos = 1;//if���� �Ѱ�ٸ� �ϴ� �̵��ؾ��ϱ⋚���� curPathPos�� 1�� ����
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


