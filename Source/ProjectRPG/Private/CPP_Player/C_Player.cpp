// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "CPP_Player/C_PlayerAnimInstance.h"
#include "CPP_Player/C_PlayerController.h"
#include "CPP_Player/C_InputQueueComponent.h"
#include "CPP_Player/C_SkillComponent.h"
#include "CPP_Player/S_InputActionData.h"
#include "CPP_Player/S_SkillData.h"
#include "Components/SceneCaptureComponent2D.h"
#include "C_InteractionDetectorComponent.h"
#include "C_TravelManagerComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Player/UI/C_PlayerSKillMGR.h"
#include "CPP_Player/UI/C_PlayerStateMGR.h"
#include "GamePlay/C_DamageWidgetComponent.h"


void AC_Player::Down()
{
	RunningState = ERunningSystemState::Down;
	DeactivateAllNiagaraEffects();
	ClearMoveState();
	bCanMove = false;
	myAnimInterface->SetIsDownMode(true);//애님인스턴스 전달
	//다운은 몽타주가아니아 애님시퀀스에서 돌아가므로 이동설정따로해야댐
}

void AC_Player::DeactivateAllNiagaraEffects()
{
	TArray<UNiagaraComponent*> NiagaraComponents;
	GetComponents<UNiagaraComponent>(NiagaraComponents);

	for (UNiagaraComponent* NiagaraComp : NiagaraComponents)
	{
		if (NiagaraComp && NiagaraComp->IsActive())
		{
			NiagaraComp->Deactivate();
		}
	}
}

void AC_Player::SetEquipMode(bool IsEquip)
{
	IsEquipMode = IsEquip;
	AttachWeaponToSocket(IsEquipMode);
	AttachDualWeaponToSocket(IsEquipMode);
}

FName AC_Player::SetPlainAttack()
{
	switch (m_nComboCount)
	{
	case 0: m_nComboCount = 1; return FName("PA_01");
	case 1: m_nComboCount = 2; return FName("PA_02");
	case 2: m_nComboCount = 0; return FName("PA_03");
	default:
		m_nComboCount = 0;
		return FName("PA_01");
	}
}

void AC_Player::ComboCountSetting(float DeltaTime)//콤보타임세팅중
{
	ComboTime += DeltaTime;
	if (ComboTime >= 2.f)
	{

	}
}

void AC_Player::HandleChargeInputStart()
{
	OnChargeStart.Broadcast();
}

void AC_Player::HandleChargeInputEnd()
{
	OnChargeEnd.Broadcast();
}

void AC_Player::HandleResult(bool result)
{
	OnResultOpen.Broadcast(result); // 성공or실패
}


void AC_Player::OnMonsterDownAttack(const FHitResult& Hit, AC_BaseCharacter* pInstigator)
{
	if (RunningState == ERunningSystemState::Down) return;
	IsDownFlying = true;
	FVector HitLocation = Hit.TraceStart;
	if (pInstigator)
		HitLocation = pInstigator->GetActorLocation();
	FVector MyLocation = GetActorLocation();

	// 맞은 방향 (내 위치에서 충돌지점 반대로)
	FVector KnockbackDir = (MyLocation - HitLocation).GetSafeNormal();
	KnockbackDir.Z = 0.f;
	DownDirection = KnockbackDir;
	if (m_fHp > 0)
	{
		Down();
	}
	DownRecive = true;
}
//void AC_Player::OnMonsterDownAttackFrom(FVector SourceLocation)
//{
//	if (RunningState == ERunningSystemState::Down) return;
//
//	IsDownFlying = true;
//
//	FVector MyLocation = GetActorLocation();
//
//	// 중앙 기준점을 내 Z 위치에 맞춰서 수평선상 계산
//	SourceLocation.Z = MyLocation.Z;
//
//	FVector KnockbackDir = (MyLocation - SourceLocation).GetSafeNormal();
//	KnockbackDir.Z = 0.f;
//
//	DownDirection = KnockbackDir;
//	Down();
//	DownRecive = true;
//}

bool AC_Player::takeDamageEvent_Implementation(float fDamage)
{
	setHp(getHp() - fDamage);
	PlayHitEffect();
	if (getHp() <= 0)
	{
		deadPlayer();
	}
	return false;
}
void AC_Player::PlayHitEffect()
{
	const FVector ActorLocation = GetActorLocation();
	// Niagara VFX
	if (HitVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, ActorLocation);
	}

	// Hit Sound
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, ActorLocation);
	}
}
//float NextHp = getHp() - fDamage;
//if (NextHp <= 0.0f)
//	fDamage = getHp();

void AC_Player::HandleChangeRunningState()
{
	if (RunningState == ERunningSystemState::Down)
	{
		bCanMove = true;
	}
	RunningState = ERunningSystemState::Idle;
	if (myAnimInterface)
	{
		myAnimInterface->SetDownPeriodState(false);
	}
	
}

void AC_Player::deadPlayer()
{
	//if (GetController())
	//	GetController()->SetActorTickEnabled(false);
	SetActorTickEnabled(false);
	SetCanBeDamaged(false);
	ClearMoveState();
	bCanMove = false;
	DownDist = 0.f;
	SetRunningSystemState(ERunningSystemState::Busy);
	m_skillCom->UsingSkill(FName("Dead"));
	if (getHp() >= 0.1f)
		setHp(0);
	if(m_onDead.IsBound())
	{
		m_onDead.Broadcast();
	}
}

void AC_Player::restartPlayer()
{
	//if (GetController())
	//	GetController()->SetActorTickEnabled(true);
	//SetActorLocation("월드상 액터위치")
	SetCanBeDamaged(true);
	SetActorTickEnabled(true);
	ClearMoveState();
	DownDist = 0.f;
	m_skillCom->UsingSkill(FName("Restart"));//bCanMove는 애니메이션노티파이에서 Enabled활성화
	SetRunningSystemState(ERunningSystemState::Idle);//idle도 몽타주가 끝나면 자동호출됨.
	setHp(getMaxHp());
	//OnMoveToPosPlayer(GetActorLocation());
	//리스타트위치?
}

void AC_Player::CalMoveData()
{
	
	if (curPathPos >= pathList.Num() || !bCanMove)//아무것도찍히지않으면 리스트의 원소개수는 1개임(현재위치)// 
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

	FVector AdjustedDirection;
	switch (DirectionSkillState)
	{
	case E4WayDirectionPlayer::Default:
		AdjustedDirection = Direction;
		break;

	case E4WayDirectionPlayer::Foward:
		AdjustedDirection = Direction;
		break;

	case E4WayDirectionPlayer::Back:
		AdjustedDirection = -Direction; // Back 벡터가 TargetPoint를 향하게
		break;

	case E4WayDirectionPlayer::Left:
		AdjustedDirection = FVector::CrossProduct(FVector::UpVector, Direction); // 왼쪽 90도 회전
		break;

	case E4WayDirectionPlayer::Right:
		AdjustedDirection = FVector::CrossProduct(Direction, FVector::UpVector); // 오른쪽 90도 회전
		break;
	}

	float TargetYaw = AdjustedDirection.Rotation().Yaw;
	float CurrentYaw = GetActorRotation().Yaw;
	float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);
	float FinalYaw = CurrentYaw + DeltaYaw;
	TargetRotationQuat = FRotator(0.f, FinalYaw, 0.f).Quaternion();
	// 틱에서 회전 보간을 켜기 위한 플래그
	bRotate = true;
}

void AC_Player::ForceLandToGround()
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (!Capsule) return;

	// 에디터에서 지정한 값 직접 사용 (절반높이 = 88, 반경 = 34)
	const float CapsuleHalfHeight = Capsule->GetScaledCapsuleHalfHeight(); // 88.0
	const float CapsuleRadius = Capsule->GetScaledCapsuleRadius();         // 34.0

	// 시작 위치를 캡슐 중심 + 하프높이 → 발바닥 기준에서 라인트레이스 쏘기
	FVector Start = GetActorLocation() + FVector(0, 0, CapsuleHalfHeight);
	FVector End = Start - FVector(0, 0, 2000.0f); // 충분히 아래로

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		// 히트한 지점에서 캡슐 절반 높이만큼 위로 올려서 캡슐 바닥이 정확히 지면에 닿도록
		FVector NewLocation = HitResult.Location + FVector(0, 0, CapsuleHalfHeight);
		SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
	}

	// 무브먼트 모드를 걷기로 변경
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->SetMovementMode(MOVE_Walking);
	}
	IsLandToGround = false;
}

//매인로직 매니저(지금 처음에 데이터를 그냥받는바람에 무제한입력이됨)
void AC_Player::RunningSystemManager()
{
	//우선순위키데이터(현제 : 패링)
	//패링은 RunningState에 영향을받지않음 즉 쿨타임을 제대로 설정하지않으면 무한으로 사용가능 
	FInputActionData PriorityInputData{};
	if (!m_inputQueue->GetLastInputData(PriorityInputData) && !m_inputQueue->GetLastChargingInputData(PriorityInputData)) return;//이부분에서 idle메인러닝시스템으로 넘어가기전에 한번 검사하는 로직
	//1. 패링 입력 최우선 처리
	if (PriorityInputData.InputType == EInputType::Period && !IsDownFlying)
	{
		if (m_skillCom->IsCooldownReady(PriorityInputData.ActionName))
		{
			//-------------------------------------------다운패링테스트---------------------------------------------------
			if (RunningState == ERunningSystemState::Down)
			{
				RunningState = ERunningSystemState::Busy;
				bCanMove = false;
				ClearMoveState();
				if (IsAttackMode)
				{
					AttackMode();
				}
				if (myAnimInterface)
				{
					myAnimInterface->SetDownPeriodState(true);
				}
				E4WayDirection Direction = Set4_WayDirection(PriorityInputData.TargetPoint);
				CalRotateData(PriorityInputData.TargetPoint);
				IsPeriod = true;
				m_skillCom->UsingSkill(FName("Period"), Direction);//이름교체
				m_skillCom->StartCooldown(PriorityInputData.ActionName);
				m_inputQueue->ClearQueueList();
				return;
			}
			// 현재 어떤 상태이든 스킬/아이템/차징 강제 중단
			RunningState = ERunningSystemState::Busy;//이 분기문을 넘어서면 바로 busy상태이므로 return반환
			DeactivateAllNiagaraEffects();
			bCanMove = false;
			ClearMoveState();
			if (IsAttackMode)
			{
				AttackMode();
			}
			CalRotateData(PriorityInputData.TargetPoint);//여기서 보간이먼저켜짐
			IsPeriod = true;//그다음 패링이켜짐 즉 보간이먼저켜지면서 true로바뀌니 패링쪽에서 보간이 false가되기전까진 진행할수없음.
			m_skillCom->UsingSkill(PriorityInputData.ActionName);//이동로직은 플레이어쪽이라 이함수는 단지 몽타주실행과 쿨타임관리만있음.
			m_skillCom->StartCooldown(PriorityInputData.ActionName);
			m_inputQueue->ClearQueueList(); // 패링 처리 후 큐 초기화
			return; // 여기서 바로 종료 (다른 입력 무시)
		}
		else
		{
			// 쿨타임 중  입력 무시 or UI에 알려줄 수 있음
			m_skillCom->skillCoolTimeTriggered(PriorityInputData.ActionName);
			return;
		}
		
	}
	if (RunningState == ERunningSystemState::Idle)
	{
		FInputActionData CurrentInputData{};//비어있는 초기값.
		if (m_inputQueue->GetLastInputData(CurrentInputData))//인풋에 뭔가 들어온다면
		{
			PriorityInputData.TargetPoint;

			m_inputQueue->ClearQueueList();//그냥 마지막인덱스를 가져온거기때문에 끝나고 다시 큐에서 가져옴 그렇기떄문에 가져오고나서 리스트를비워줘야 끝나고 자동으로 가져오지않음.
			switch (CurrentInputData.InputType)
			{
			case EInputType::PlainAttack:
				RunningState = ERunningSystemState::Busy;
				AttackMode();//스킬쓰면 어택킹모드진입
				if (myAnimInterface)
				{
					myAnimInterface->SetAttackMode(true);
				}
				bCanMove = false;
				ClearMoveState();
				CalRotateData(CurrentInputData.TargetPoint);
				m_skillCom->UsingSkill(SetPlainAttack());
				break;
			case EInputType::Skill:
				if (m_skillCom->IsCooldownReady(CurrentInputData.ActionName))
				{
					RunningState = ERunningSystemState::Busy;
					AttackMode();//스킬쓰면 어택킹모드진입
					if (myAnimInterface)
					{
						myAnimInterface->SetAttackMode(true);
					}
					bCanMove = false;//움직임 제어(애니메이션이 끝날때 다시 트루로 바꿔주는 함수호출)
					ClearMoveState();
					CalRotateData(CurrentInputData.TargetPoint);//보간함수->틱보간
					m_skillCom->UsingSkill(CurrentInputData.ActionName);//컨트롤러에서 만들어진 name과 구조체안 스킬name이 같아야함.
					//쿨타임 시작
					m_skillCom->StartCooldown(CurrentInputData.ActionName);
				}
				else
				{
					// 쿨타임 중  입력 무시 or UI에 알려줄 수 있음
					m_skillCom->skillCoolTimeTriggered(PriorityInputData.ActionName);
					return;
				}
				break;
			case EInputType::AnimItem:
				RunningState = ERunningSystemState::Busy;
				CalRotateData(CurrentInputData.TargetPoint);
				//ExecuteSkill(CurrentInputData); ->실행함수
				break;
			case EInputType::ChargeSkill:
				if (m_skillCom->IsCooldownReady(CurrentInputData.ActionName))
				{
					RunningState = ERunningSystemState::Charging;
					AttackMode();//스킬쓰면 어택킹모드진입
					if (myAnimInterface)
					{
						myAnimInterface->SetAttackMode(true);
					}
					bCanMove = false;//움직임 제어(애니메이션이 끝날때 다시 트루로 바꿔주는 함수호출)
					ClearMoveState();
					CalRotateData(CurrentInputData.TargetPoint);//보간함수->틱보간
					if (SkillUiWidget)
					{
						SkillUiWidget->ShowPerfectZone();
					}
					m_inputQueue->ClearChargingQueueList();//혹시 이전에쓰고 아직안비워져있을수있으니
					m_inputQueue->StartChargingSet();
					m_skillCom->UsingSkill(CurrentInputData.ActionName);//컨트롤러에서 만들어진 name과 구조체안 스킬name이 같아야함.
					//쿨타임 시작
					m_skillCom->StartCooldown(CurrentInputData.ActionName);
					HandleChargeInputStart();
				}
				else
				{
					// 쿨타임 중  입력 무시 or UI에 알려줄 수 있음
					m_skillCom->skillCoolTimeTriggered(PriorityInputData.ActionName);
					return;
				}
				break;

			default:
				break;
			}
		}
	}
	else if (RunningState == ERunningSystemState::Charging)//이미idle에서 차징스타트로 상태변경되서넘어옴
	{
		FInputActionData ChargeInput{};
		if (m_inputQueue->GetLastChargingInputData(ChargeInput))//계속 해당차징스킬데이터입력이들어올것임(인덱스번호든, Trigged이든)
		{
			//같은 인풋타입은 charing이지만 이미 누른순간 스타트는 idle상태에서 인식하고 넘어왔기에 이제 남은건 (Held,Canceld,Completed)
			switch (ChargeInput.InputStateType)
			{
			case EInputStateType::Held:
				//홀딩은 이미 애님인스턴스에서 루프중임
				break;
			case EInputStateType::Released://캔슬과 완료일때 모두 Released가 세팅됨
				RunningState = ERunningSystemState::Busy;
				m_inputQueue->ClearChargingQueueList();
				HandleChargeInputEnd();
				if (ChargeInput.Timing == false)
				{
					HandleResult(ChargeInput.Timing);
					//실패! 브로드캐스트( 몽타주[실패이펙트,사운드], UI실패)
				}
				else 
				{
					HandleResult(ChargeInput.Timing);
					//성공!
				}
				m_skillCom->RequestJumpToSection(FName("Released"));
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
	FVector DirectionVector = GetActorForwardVector();
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	bool bIsAirborne = MoveComp && !MoveComp->IsMovingOnGround(); // 공중 체크 (Flying/Falling)
	// Flying 상태일 때 공중 패링용 처리
	if (bIsAirborne)
	{
		// 캐릭터 Forward 기준 아래 40도 방향 계산
		FRotator DownRot = DirectionVector.Rotation();
		DownRot.Pitch -= 45.f; // 아래로 40도
		FVector TraceDir = DownRot.Vector();

		// 라인트레이스
		FVector Start = GetActorLocation();
		FVector End = Start + TraceDir * 2000.f; // 충분히 아래

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
		{
			// 히트 위치 + 캡슐 반높이
			float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			FVector TargetPos = HitResult.Location + FVector(0, 0, CapsuleHalfHeight);

			// Parry 방향 설정
			ParryDirection = (TargetPos - GetActorLocation()).GetSafeNormal();

			// PeriodDist 계산 (히트 지점까지 거리)
			PeriodDist = FVector::Dist(GetActorLocation(), TargetPos);
		}
		//else
		//{
		//	// 히트 못하면 기존 Forward
		//	ParryDirection = DirectionVector;
		//	PeriodDist = 300.f; // 기본값
		//}

	}
	else
	{
		// 일반 지상 패링 처리
		switch (DirectionSkillState)
		{
		case E4WayDirectionPlayer::Foward:
			DirectionVector = GetActorForwardVector();
			break;
		case E4WayDirectionPlayer::Back:
			DirectionVector = -GetActorForwardVector();
			break;
		case E4WayDirectionPlayer::Left:
			DirectionVector = -GetActorRightVector();
			break;
		case E4WayDirectionPlayer::Right:
			DirectionVector = GetActorRightVector();
			break;
		case E4WayDirectionPlayer::Default:
			DirectionVector = GetActorForwardVector();
			break;
		}
		DirectionVector.Z = 0.0f;
		ParryDirection = DirectionVector.GetSafeNormal();
		UE_LOG(LogTemp, Warning, TEXT("WalkPeriod"));
	}
	IsPeriod = true;
}

void AC_Player::AttachWeaponToSocket(bool bEquipMode)
{
	if (!EquippedWeapon) return;

	FName SocketName = bEquipMode ? FName("r_Equip_Socket") : FName("r_weapon_socket");
	EquippedWeapon->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		SocketName
	);
}

void AC_Player::AttachDualWeaponToSocket(bool bEquipMode)
{
	if (!EquippedWeaponDual) return;

	FName SocketName = bEquipMode ? FName("l_Equip_Socket") : FName("l_weapon_socket");
	EquippedWeaponDual->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		SocketName
	);
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
		//m_pPlayerInfoCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PlayerInfoCaptureComponent"));
		//m_pPlayerInfoCaptureComponent->SetupAttachment(GetRootComponent());
		//m_pPlayerInfoCaptureComponent->SetRelativeLocation(FVector{ 180,0,20 });
		//m_pPlayerInfoCaptureComponent->SetRelativeRotation(FRotator{ -4,180,0 });
		//m_pPlayerInfoCaptureComponent->FOVAngle = 75.0f;
		m_pInteractionDetectComponent = CreateDefaultSubobject<UC_InteractionDetectorComponent>(TEXT("InteractionDetectComponent"));
		m_pTravelComponent = CreateDefaultSubobject<UC_TravelManagerComponent>(TEXT("TravelComponent"));
		//Script/Engine.TextureRenderTarget2D'/Game/UI/PlayerInfo/Texture/T_PlayerInfo.T_PlayerInfo'
		//static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RenderTarget(TEXT("/Game/UI/PlayerInfo/Texture/T_PlayerInfo.T_PlayerInfo"));
		//if (RenderTarget.Succeeded())
		//{
		//	m_pPlayerInfoCaptureComponent->TextureTarget = RenderTarget.Object;
		//}
		//m_pPlayerInfoCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
		m_pInteractionDetectComponent->SetupAttachment(GetRootComponent());

		m_pDamageWidget = CreateDefaultSubobject< UC_DamageWidgetComponent>(TEXT("DamageWidget"));
		m_pDamageWidget->SetupAttachment(GetRootComponent());
	}
	
}

void AC_Player::BeginPlay()
{
	Super::BeginPlay();
	//m_pPlayerInfoCaptureComponent->ShowOnlyActorComponents(this);
	
	if (USkeletalMeshComponent* myMesh = GetMesh())
	{
		myMesh->SetRenderCustomDepth(true);
		UC_PlayerAnimInstance* myAnimInstance = Cast<UC_PlayerAnimInstance>(myMesh->GetAnimInstance());
		if (myAnimInstance)
		{
			//무기 파지법
			myAnimInstance->OnWeaponModeChanged.AddDynamic(this, &AC_Player::SetEquipMode);

			
			//myAnimInterface에 플레이어의 애님인스턴스의 인터페이스 참조세팅
			myAnimInterface.SetObject(myAnimInstance);                 
			myAnimInterface.SetInterface(Cast<II_PlayerToAnimInstance>(myAnimInstance));
			// 여기서 델리게이트 바인딩
			myAnimInstance->ChangeRunningState.RemoveAll(this);//안전장치(예를들어 캐릭터가 죽고 다시살아날떄.
			myAnimInstance->SetPlayerMovePointEnabled.RemoveAll(this);

			myAnimInstance->ChangeRunningState.AddUObject(this, &AC_Player::HandleChangeRunningState);
			myAnimInstance->SetPlayerMovePointEnabled.AddUObject(this, &AC_Player::SetCanMove);

			//이제 노티파이발생시 애님인스턴스에서 브로드캐스트로 플레이어에게 전달
			//플레이어는 바인딩된 'HandleChangeRunningState' 실핼
		}
	}


	if (WeaponClass)
	{
		// 무기 스폰
		EquippedWeapon = GetWorld()->SpawnActor<AActor>(WeaponClass);
		if (EquippedWeapon)
		{
			AttachWeaponToSocket(IsEquipMode);
		}
	}
	if (DualWeaponClass)
	{
		// 무기 스폰
		EquippedWeaponDual = GetWorld()->SpawnActor<AActor>(DualWeaponClass);
		if (EquippedWeaponDual)
		{
			AttachDualWeaponToSocket(IsEquipMode);
		}
	}
	//UI
	if (SkillUiClass) // SkillUiClass는 UClass를 가리킴
	{
		SkillUiWidget = CreateWidget<UC_PlayerSKillMGR>(GetWorld(), SkillUiClass);
		if (SkillUiWidget)
		{
			SkillUiWidget->AddToViewport();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%f"), m_fMaxHp);
	UE_LOG(LogTemp, Warning, TEXT("%f"), m_fHp);
	UE_LOG(LogTemp, Warning, TEXT("%f"), m_fAtk);

}

void AC_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerStateCheking(DeltaTime);


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
			//여기서 계속 꺼트려줌 ->SetInfo에서 세팅을하고 켜줘야 넘어감.
			DirectionSkillState = E4WayDirectionPlayer::Default;
			IsPeriod = false;
		}
		else
		{
			float speed = 50.f;
			FVector MoveVec = ParryDirection * speed;
			AddActorWorldOffset(MoveVec, true);
			PeriodDist -= MoveVec.Length();
		}
		
	}
	if (DownRecive)
	{
		ClearMoveState();
		if (DownDist < 0.2f)//도착
		{
			DownDist = 700.f;
			IsDownFlying = false;
			DownRecive = false;
		}
		else
		{
			float speed = 50.f;
			FVector MoveVec = DownDirection * speed;
			AddActorWorldOffset(MoveVec, true);
			DownDist -= MoveVec.Length();
		}
		UE_LOG(LogTemp, Warning, TEXT("DownDist %f"), DownDist);
	}
	RunningSystemManager();

}

void AC_Player::OnMoveToPosPlayer(FVector pos)
{
	if (!bCanMove) return;//idle스테이트가 아니면 리턴시킴(마우스포인터로 찍히지만 실제이동은되지않도록)

	FVector FinalPos = pos;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	bool bOnNavmesh = false;
	if (NavSys)
	{
		FNavLocation ProjectedLocation;
		if (NavSys->ProjectPointToNavigation(pos, ProjectedLocation, FVector(200.f, 200.f, 500.f)))
		{
			FinalPos = ProjectedLocation.Location;
			bOnNavmesh = true;
		}
	}
	if (bOnNavmesh)
	{
		//길찾기 패스 구하기
		UNavigationPath* Path =
			UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), FinalPos);
		if (Path != nullptr && Path->IsValid() && Path->PathPoints.Num() > 1)//네브메시볼륨에 찍혔고, 유효하고, 패스포인트가 2개이상인경우
		{
			pathList = Path->PathPoints;//리스트에 포인팅된 path정보를 담음(장애물이없다면 리스트에 2개가담김->[현재위치][찍은위치]
			curPathPos = 1;//if문을 넘겼다면 일단 이동해야하기떄문에 curPathPos를 1로 설정
		}
	}

	else
	{
		{
			// 네브메시에 없는 경우 → 그냥 직선 이동
			pathList.Empty();
			pathList.Add(GetActorLocation()); // 시작점
			pathList.Add(FinalPos);           // 목적지
			curPathPos = 1;
		}
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

E4WayDirection AC_Player::Set4_WayDirection(const FVector& mousePoint)
{
	FVector ToMouse = mousePoint - GetActorLocation();
	ToMouse.Z = 0;
	ToMouse.Normalize();

	FVector Forward = GetActorForwardVector();
	Forward.Z = 0;
	Forward.Normalize();

	float Dot = FVector::DotProduct(Forward, ToMouse);//0~1사이값 =>각도를 얻어옴
	float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(Dot));//라디안값으로 각도로 변환
	//캐릭터 포워드기준으로 오른쪽은 90도 왼쪽은 -90도

	// CrossProduct는 왼쪽 / 오른쪽 판별용으로 사용//오른손법칙 z>0 ->왼쪽, z<0 ->오른쪽
	float CrossZ = FVector::CrossProduct(Forward, ToMouse).Z;
	E4WayDirectionPlayer Direction;
	if (AngleDegrees <= 45.0f)
	{
		Direction = E4WayDirectionPlayer::Foward;
	}
	else if (AngleDegrees > 135.0f)
	{
		Direction = E4WayDirectionPlayer::Back;
	}
	else
	{
		// 90도 ±45도는 옆방향, CrossZ로 왼/오 구분
		if (CrossZ > 0)
		{
			Direction = E4WayDirectionPlayer::Right;
		}
		else
		{
			Direction = E4WayDirectionPlayer::Left;
		}
	}
	//따로 상태저장
	DirectionSkillState = Direction;
	// 로그 출력 (테스트용)
	FString DirString;
	switch (Direction)
	{
	case E4WayDirectionPlayer::Foward: DirString = TEXT("Forward"); break;
	case E4WayDirectionPlayer::Back:   DirString = TEXT("Back"); break;
	case E4WayDirectionPlayer::Left:   DirString = TEXT("Left"); break;
	case E4WayDirectionPlayer::Right:  DirString = TEXT("Right"); break;
	}
	UE_LOG(LogTemp, Warning, TEXT("4-Way Direction: %s"), *DirString);
	
	switch (Direction)
	{
	case E4WayDirectionPlayer::Foward:
		return E4WayDirection::Forward;
	case E4WayDirectionPlayer::Back:
		return E4WayDirection::Back;
	case E4WayDirectionPlayer::Left:
		return E4WayDirection::Left;
	case E4WayDirectionPlayer::Right:
		return E4WayDirection::Right;
	}
	return E4WayDirection::Default;
}
void AC_Player::AttackMode()
{
	if (IsAttackMode)
	{
		IsAttackMode = false;
		AttackingModeTime = 0.f;
		IsAttackMode = true;
	}
	else if (!IsAttackMode)
	{
		IsAttackMode = true;
		if (IsEquipMode)
		{
			//무기파지
			IsEquipMode = false;
			AttachWeaponToSocket(IsEquipMode);
			AttachDualWeaponToSocket(IsEquipMode);
		}
	}
	
}
void AC_Player::PlayerStateCheking(float DeltaTime)
{
	if (!IsAttackMode) return;//아이들모드면 체킹할필요없음
	AttackingModeTime += DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("ReturnIdleMode %f"), AttackingModeTime);

	if (AttackingModeTime > 5.f)
	{
		IsAttackMode = false;//다시 아이들모드로 되돌림.
		AttackingModeTime = 0.f;
		myAnimInterface->SetAttackMode(false);
		UE_LOG(LogTemp, Warning, TEXT("ReturnIdleMode %f"), AttackingModeTime);
		return;
	}

}

void AC_Player::runInteraction()
{
	if (m_pInteractionDetectComponent)
	{
		m_pInteractionDetectComponent->runInteraction();
	}
}