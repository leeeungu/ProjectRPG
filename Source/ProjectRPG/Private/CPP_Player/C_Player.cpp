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
#include "Components/SceneCaptureComponent2D.h"
#include "C_InteractionDetectorComponent.h"
#include "C_TravelManagerComponent.h"
#include "Engine/TextureRenderTarget2D.h"

void AC_Player::HandleChargingReady(bool bIsReady)
{
	bChargingReady = bIsReady;
}

void AC_Player::HandleChangeRunningState()
{
	RunningState = ERunningSystemState::Idle;
}

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

//매인로직 매니저
void AC_Player::RunningSystemManager()
{
	//우선순위키데이터(현제 : 패링)
	//패링은 RunningState에 영향을받지않음 즉 쿨타임을 제대로 설정하지않으면 무한으로 사용가능 
	FInputActionData PriorityInputData{};
	if (m_inputQueue->GetLastInputData(PriorityInputData))//이부분에서 idle메인러닝시스템으로 넘어가기전에 한번 검사하는 로직
	{
		//1. 패링 입력 최우선 처리
		if (PriorityInputData.InputType == EInputType::Period)
		{
			// 현재 어떤 상태이든 스킬/아이템/차징 강제 중단
			//InterruptAllActions();   //진행중인 모든몽타주 stop
			RunningState = ERunningSystemState::Busy;//이 분기문을 넘어서면 바로 busy상태이므로 return반환
			bHoldSkillPlayed = false;
			bCanMove = false;
			CalRotateData(PriorityInputData.TargetPoint);//여기서 보간이먼저켜짐
			IsPeriod = true;//그다음 패링이켜짐 즉 보간이먼저켜지면서 true로바뀌니 패링쪽에서 보간이 false가되기전까진 진행할수없음.
			m_skillCom->UsingSkill(PriorityInputData.ActionName);//이동로직은 플레이어쪽이라 이함수는 단지 몽타주실행과 쿨타임관리만있음.
			m_inputQueue->ClearQueueList(); // 패링 처리 후 큐 초기화
			return; // 여기서 바로 종료 (다른 입력 무시)
		}
	}
	if (RunningState == ERunningSystemState::Idle)
	{
		FInputActionData CurrentInputData{};//비어있는 초기값.
		if (m_inputQueue->GetLastInputData(CurrentInputData))//인풋에 뭔가 들어온다면
		{
			m_inputQueue->ClearQueueList();//그냥 마지막인덱스를 가져온거기때문에 끝나고 다시 큐에서 가져옴 그렇기떄문에 가져오고나서 리스트를비워줘야 끝나고 자동으로 가져오지않음.
			switch (CurrentInputData.InputType)
			{
			case EInputType::Skill:
				RunningState = ERunningSystemState::Busy;
				bCanMove = false;//움직임 제어(애니메이션이 끝날때 다시 트루로 바꿔주는 함수호출)
				CalRotateData(CurrentInputData.TargetPoint);//보간함수->틱보간
				m_skillCom->UsingSkill(CurrentInputData.ActionName);//컨트롤러에서 만들어진 name과 구조체안 스킬name이 같아야함.
				break;
			case EInputType::AnimItem:
				RunningState = ERunningSystemState::Busy;
				CalRotateData(CurrentInputData.TargetPoint);
				//ExecuteSkill(CurrentInputData); ->실행함수
				break;
			case EInputType::ChargeSkill:
				RunningState = ERunningSystemState::Charging;
				bCanMove = false;
				CalRotateData(CurrentInputData.TargetPoint);
				m_inputQueue->ClearChargingQueueList();//혹시 이전에쓰고 아직안비워져있을수있으니
				m_skillCom->UsingSkill(CurrentInputData.ActionName);
				//StartChargeSkill(CurrentInputData);->차징시작함수(시간계산필요, 몽타주홀딩필요)
				break;

			default:
				break;
			}
		}
	}
	else if (RunningState == ERunningSystemState::Charging)//이미idle에서 차징스타트로 상태변경되서넘어옴
	{
		//if (!bChargingReady)
		//{
		//	// 스타트 몽타주 끝나기 전에는 아무 입력도 처리하지 않음
		//	return;
		//}
		// 차징 스킬 입력만 허용 -> 나머지 무시
		FInputActionData ChargeInput{};
		if (m_inputQueue->GetLastChargingInputData(ChargeInput))//계속 해당차징스킬데이터입력이들어올것임(인덱스번호든, Trigged이든)
		{
			//같은 인풋타입은 charing이지만 이미 누른순간 스타트는 idle상태에서 인식하고 넘어왔기에 이제 남은건 (Held,Canceld,Completed)
			switch (ChargeInput.InputStateType)
			{
			case EInputStateType::Held:
				//if (!bHoldSkillPlayed)
				//{
				//	m_skillCom->UsingSkill(ChargeInput.ActionName);
				//	bHoldSkillPlayed = true; // 이후에는 무시
				//}
				//홀딩은 이미 애님인스턴스에서 루프중임
				break;
			case EInputStateType::Released://캔슬과 완료일때 모두 Released가 세팅됨
				//m_skillCom->UsingSkill(ChargeInput.ActionName);
				//UE_LOG(LogTemp, Warning, TEXT("Start Montage Finished → Charging End!"));
				//bHoldSkillPlayed = false;
				//bChargingReady = false; // 플래그 초기화
				//m_inputQueue->ClearChargingQueueList();//중간에 홀딩때 패링같은걸써서 큐클리어못하면? 
				m_inputQueue->ClearChargingQueueList();
				if (m_skillCom)
				{
					m_skillCom->RequestJumpToSection(FName("Released"));
				}
				break;
			}
		}
	}
	else if (RunningState == ERunningSystemState::Busy || RunningState == ERunningSystemState::Down)
	{
		return;//일반 리턴으로 처리(만약에 스킬사용중이나 차징스킬사용중에 뭔가 입력을 받아야한다면 그냥 바로이벤트로 처리(큐에 add X)
	}

}

UCameraComponent* AC_Player::getCamera_Implementation()
{
	return m_camCom;
}

void AC_Player::Reset_Implementation(UCameraComponent* Camera)
{
	if (m_camCom)
		m_camCom->AttachToComponent(m_springCom,FAttachmentTransformRules::KeepRelativeTransform);
	else if (Camera)
		Camera->AttachToComponent(m_springCom,FAttachmentTransformRules::KeepRelativeTransform);
}

void AC_Player::SetPeriodInfo()
{
	FVector Forward = GetActorForwardVector();
	Forward.Z = 0.0f;
	ParryDirection = Forward.GetSafeNormal();
	IsPeriod = true;
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
	{
		m_pPlayerInfoCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PlayerInfoCaptureComponent"));
		m_pInteractionDetectComponent = CreateDefaultSubobject<UC_InteractionDetectorComponent>(TEXT("InteractionDetectComponent"));
		m_pTravelComponent = CreateDefaultSubobject<UC_TravelManagerComponent>(TEXT("TravelComponent"));
		m_pPlayerInfoCaptureComponent->SetupAttachment(GetRootComponent());
		m_pPlayerInfoCaptureComponent->SetRelativeLocation(FVector{ 180,0,20 });
		m_pPlayerInfoCaptureComponent->SetRelativeRotation(FRotator{ -4,180,0 });
		m_pPlayerInfoCaptureComponent->FOVAngle = 75.0f;
		//Script/Engine.TextureRenderTarget2D'/Game/UI/PlayerInfo/Texture/T_PlayerInfo.T_PlayerInfo'
		static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RenderTarget(TEXT("/Game/UI/PlayerInfo/Texture/T_PlayerInfo.T_PlayerInfo"));
		if (RenderTarget.Succeeded())
		{
			m_pPlayerInfoCaptureComponent->TextureTarget = RenderTarget.Object;
		}
		m_pPlayerInfoCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
		m_pInteractionDetectComponent->SetupAttachment(GetRootComponent());
	}
}

void AC_Player::BeginPlay()
{
	Super::BeginPlay();
	m_pPlayerInfoCaptureComponent->ShowOnlyActorComponents(this);

	if (USkeletalMeshComponent* myMesh = GetMesh())
	{
		UC_PlayerAnimInstance* myAnimInstance = Cast<UC_PlayerAnimInstance>(myMesh->GetAnimInstance());
		if (myAnimInstance)
		{
			// 여기서 델리게이트 바인딩
			myAnimInstance->ChangeRunningState.RemoveAll(this);//안전장치(예를들어 캐릭터가 죽고 다시살아날떄.
			myAnimInstance->SetPlayerMovePointEnabled.RemoveAll(this);
			myAnimInstance->ChargingReadyChanged.RemoveAll(this);
			myAnimInstance->ChangeRunningState.AddUObject(this, &AC_Player::HandleChangeRunningState);
			myAnimInstance->SetPlayerMovePointEnabled.AddUObject(this, &AC_Player::SetCanMove);
			myAnimInstance->ChargingReadyChanged.AddUObject(this, &AC_Player::HandleChargingReady);
			//이제 노티파이발생시 애님인스턴스에서 브로드캐스트로 플레이어에게 전달
			//플레이어는 바인딩된 'HandleChangeRunningState' 실핼
		}
	}

}

void AC_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FString StateName;

	switch (RunningState)
	{
	case ERunningSystemState::Idle:
		StateName = TEXT("Idle");
		break;
	case ERunningSystemState::Busy:
		StateName = TEXT("Busy");
		break;
	case ERunningSystemState::Charging:
		StateName = TEXT("Charging");
		break;
	case ERunningSystemState::Down:
		StateName = TEXT("Down");
		break;
	default:
		StateName = TEXT("Unknown");
		break;
	}

	// 화면 좌측 상단에 텍스트 출력 (Key: -1 은 항상 새로 출력됨)
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			0.f,                     // Duration: 0초면 매 프레임 다시 출력
			FColor::Green,
			FString::Printf(TEXT("[Player State] RunningState: %s"), *StateName)
		);
	}

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
	//마우스포인터위치로 보간회전(보통 스킬쓸때나 해당위치로 몸을돌리는작업할떄만 열림/
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
	//패링
	if (IsPeriod && !bRotate)//보간이끝나고 정면을 바라봤을떄 패링이 진행되도록
	{
		SetPeriodInfo();
		
		remainDist = 0.f;
		if (PeriodDist < 0.2f)//도착
		{
			PeriodDist = 300.f;
			IsPeriod = false;
		}
		else
		{
			float speed = 50.f;
			FVector MoveVec = ParryDirection * speed;
			AddActorWorldOffset(MoveVec, true);
			PeriodDist -= MoveVec.Length();
			UE_LOG(LogTemp, Warning, TEXT("Period true"));
		}
	}
	RunningSystemManager();

}

void AC_Player::OnMoveToPosPlayer(FVector pos)
{
	if (!bCanMove) return;//idle스테이트가 아니면 리턴시킴(마우스포인터로 찍히지만 실제이동은되지않도록)
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

FVector AC_Player::GetMousePointDir()
{
	return this->MousePointDir;
}

void AC_Player::ClearMoveState()
{
	pathList.Empty();
	CalMoveData();
}

void AC_Player::Set4_WayDirection(const FVector& mousePoint)
{
	FVector ToMouse = mousePoint - GetActorLocation();
	ToMouse.Z = 0;
	ToMouse.Normalize();

	FVector Forward = GetActorForwardVector();
	Forward.Z = 0;
	Forward.Normalize();

	// DotProduct는 두 벡터 사이의 각도 관계를 알 수 있음
	float Dot = FVector::DotProduct(Forward, ToMouse);
	float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(Dot));

	// CrossProduct는 왼쪽 / 오른쪽 판별용으로 사용
	float CrossZ = FVector::CrossProduct(Forward, ToMouse).Z;

	E4WayDirection Direction;

	if (AngleDegrees <= 45.0f)
	{
		Direction = E4WayDirection::Foward;
	}
	else if (AngleDegrees > 135.0f)
	{
		Direction = E4WayDirection::Back;
	}
	else
	{
		// 90도 ±45도는 옆방향, CrossZ로 왼/오 구분
		if (CrossZ > 0)
		{
			Direction = E4WayDirection::Left;
		}
		else
		{
			Direction = E4WayDirection::Right;
		}
	}

	// 로그 출력 (테스트용)
	FString DirString;
	switch (Direction)
	{
	case E4WayDirection::Foward: DirString = TEXT("Forward"); break;
	case E4WayDirection::Back:   DirString = TEXT("Back"); break;
	case E4WayDirection::Left:   DirString = TEXT("Left"); break;
	case E4WayDirection::Right:  DirString = TEXT("Right"); break;
	}

	UE_LOG(LogTemp, Warning, TEXT("4-Way Direction: %s"), *DirString);

	// 여기에 멤버 변수 세팅 또는 추가 로직
	// Direction; // 디렉션을 저장하든 넘겨주든하면됨.
}


