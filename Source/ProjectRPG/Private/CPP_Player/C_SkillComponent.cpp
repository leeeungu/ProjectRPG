// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_SkillComponent.h"
#include "CPP_Player/S_SkillData.h"
#include "CPP_Player/C_PlayerAnimInstance.h"
#include "C_MonsterBaseCharacter.h"
#include "C_Basecharacter.h"
#include "GameFramework/Character.h"
#include "C_GameAlertSubsystem.h"

void UC_SkillComponent::SpawnSkillCollision(const FSkillCollisionData& CollisionData, FVector skillLocation, FRotator skillRotation, bool IsGetCounter)
{
	FVector SpawnLocation = skillLocation;
	FQuat SpawnQuat = skillRotation.Quaternion();

	FCollisionShape CollisionShape;
	switch (CollisionData.ShapeType)
	{
	case ESkillCollisionShapeType::Sphere:
		CollisionShape = FCollisionShape::MakeSphere(CollisionData.Dimensions.X);
		break;
	case ESkillCollisionShapeType::Box:
		CollisionShape = FCollisionShape::MakeBox(CollisionData.Dimensions * 0.5f);
		break;
	case ESkillCollisionShapeType::Capsule:
		CollisionShape = FCollisionShape::MakeCapsule(CollisionData.Dimensions.X, CollisionData.Dimensions.Z);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown Collision ShapeType"));
		return;
	}
	FVector Start = SpawnLocation;
	FVector End = SpawnLocation + FVector(0.1f, 0, 0); // 아주 작은 이동
	// 데미지 적용 로직 예시
	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		SpawnQuat,
		ECC_Pawn,
		CollisionShape
	);



#ifdef DEBUG_DRAW
	switch (CollisionData.ShapeType)
	{
	case ESkillCollisionShapeType::Sphere:
		DrawDebugSphere(GetWorld(), SpawnLocation, CollisionData.Dimensions.X, 16, FColor::Green, false, 2.f);
		break;
	case ESkillCollisionShapeType::Box:
		DrawDebugBox(GetWorld(), SpawnLocation, CollisionData.Dimensions * 0.5f, SpawnQuat, FColor::Blue, false, 2.f);
		break;
	case ESkillCollisionShapeType::Capsule:
		DrawDebugCapsule(GetWorld(), SpawnLocation, CollisionData.Dimensions.Z, CollisionData.Dimensions.X, SpawnQuat, FColor::Red, false, 2.f);
		break;
	}
#endif
	//블록?, 오버랩? 
	if (bHit)
	{
		//get owner
		for (const FHitResult& Hit : HitResults)
		{
			AC_MonsterBaseCharacter* HitActor = Cast<AC_MonsterBaseCharacter>(Hit.GetActor());
			if (HitActor && HitActor != GetOwner())
			{
				float Amount = DamageAmount(CurrentSkillName);
				float Stagger = StaggerAmount(CurrentSkillName);
				HitActor->takeDamageEvent(Amount);
				HitActor->takeStaggerEvent(Stagger);
			}
			if (IsGetCounter)
			{
				AC_MonsterBaseCharacter* BossMonster = Cast<AC_MonsterBaseCharacter>(Hit.GetActor());
				if (BossMonster)
				{
					BossMonster->tryCounter();
				}
			}
			
		}
	}
}

float UC_SkillComponent::DamageAmount(FName SkillName)
{
	float Amount{};
	if (const FSkillData* Skill = SkillMap.Find(SkillName))
	{
		// Owner 가져오기
		if (AC_BaseCharacter* OwnerChar = Cast<AC_BaseCharacter>(GetOwner()))
		{
			// 캐릭터 클래스에 GetAtk() 함수가 있다고 가정
			float BaseAtk = OwnerChar->getAtk();
			Amount = BaseAtk * Skill->AttackPowerMultiplier;
		}
	}
	return Amount;
}

float UC_SkillComponent::StaggerAmount(FName SkillName)
{
	float Amount{};
	if (const FSkillData* Skill = SkillMap.Find(SkillName))
	{
		// Owner 가져오기
		if (AC_BaseCharacter* OwnerChar = Cast<AC_BaseCharacter>(GetOwner()))
		{
			Amount = Skill->AttackPowerMultiplier;//무력화값으로 변경
		}
	}
	return Amount;
}



// Sets default values for this component's properties
UC_SkillComponent::UC_SkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//평타
	FSkillData PlainAttack01{};
	PlainAttack01.SkillName = "PA_01";
	PlainAttack01.Cooldown = 0.0f;//테스트용 0초
	PlainAttack01.AttackPowerMultiplier = 2.1f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PA1obj(TEXT("/Game/RPG_Hero_Animation(v2)/PlainAttack/Dual_Plain01_Montage.Dual_Plain01_Montage"));
	if (PA1obj.Succeeded()) PlainAttack01.DirectionMontages.Add(E4WayDirection::Default, PA1obj.Object);
	SkillMap.Add(PlainAttack01.SkillName, PlainAttack01);

	FSkillData PlainAttack02{};
	PlainAttack02.SkillName = "PA_02";
	PlainAttack02.Cooldown = 0.0f;//테스트용 0초
	PlainAttack02.AttackPowerMultiplier = 2.1f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PA2obj(TEXT("/Game/RPG_Hero_Animation(v2)/PlainAttack/Dual_Plain02_Montage.Dual_Plain02_Montage"));
	if (PA1obj.Succeeded()) PlainAttack02.DirectionMontages.Add(E4WayDirection::Default, PA2obj.Object);
	SkillMap.Add(PlainAttack02.SkillName, PlainAttack02);

	FSkillData PlainAttack03{};
	PlainAttack03.SkillName = "PA_03";
	PlainAttack03.Cooldown = 0.0f;//테스트용 0초
	PlainAttack03.AttackPowerMultiplier = 2.1f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PA3obj(TEXT("/Game/RPG_Hero_Animation(v2)/PlainAttack/Dual_Plain03_Montage.Dual_Plain03_Montage"));
	if (PA1obj.Succeeded()) PlainAttack03.DirectionMontages.Add(E4WayDirection::Default, PA3obj.Object);
	SkillMap.Add(PlainAttack03.SkillName, PlainAttack03);

	//Q스킬
	FSkillData SkillNum01{};
	SkillNum01.SkillName = "S_01";
	SkillNum01.Cooldown = 2.0f;//테스트용 0초
	SkillNum01.AttackPowerMultiplier = 3.3f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill1obj(TEXT("/Game/RPG_Hero_Animation(v2)/Skill/Dual_Skill01_Montage.Dual_Skill01_Montage"));
	if (skill1obj.Succeeded()) SkillNum01.DirectionMontages.Add(E4WayDirection::Default, skill1obj.Object);
	SkillMap.Add(SkillNum01.SkillName, SkillNum01);

	FSkillData SkillNum02{};
	SkillNum02.SkillName = "S_02";
	SkillNum02.Cooldown = 5.0f;
	SkillNum02.AttackPowerMultiplier = 4.5f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill2obj(TEXT("/Game/RPG_Hero_Animation(v2)/Skill/Dual_Skill02_Montage.Dual_Skill02_Montage"));
	if (skill2obj.Succeeded()) SkillNum02.DirectionMontages.Add(E4WayDirection::Default, skill2obj.Object);
	SkillMap.Add(SkillNum02.SkillName, SkillNum02);

	FSkillData SkillNum03{};
	SkillNum03.SkillName = "S_03";
	SkillNum03.Cooldown = 5.0f;
	SkillNum03.AttackPowerMultiplier = 5.6f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill3obj(TEXT("/Game/RPG_Hero_Animation(v2)/Skill/Dual_Skill03_Montage.Dual_Skill03_Montage"));
	if (skill3obj.Succeeded()) SkillNum03.DirectionMontages.Add(E4WayDirection::Default, skill3obj.Object);
	SkillMap.Add(SkillNum03.SkillName, SkillNum03);

	FSkillData SkillNum04{};
	SkillNum04.SkillName = "S_04";
	SkillNum04.Cooldown = 5.0f;
	SkillNum04.AttackPowerMultiplier = 7.2f;
	SkillNum04.StaggerAmount = 2.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill4obj(TEXT("/Game/RPG_Hero_Animation(v2)/Skill/Dual_Skill04_Montage.Dual_Skill04_Montage"));
	if (skill4obj.Succeeded()) SkillNum04.DirectionMontages.Add(E4WayDirection::Default, skill4obj.Object);
	SkillMap.Add(SkillNum04.SkillName, SkillNum04);

	FSkillData SkillNum05{};
	SkillNum05.SkillName = "S_05";
	SkillNum05.Cooldown = 3.0f;
	SkillNum05.AttackPowerMultiplier = 1.5f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill5obj(TEXT("/Game/RPG_Hero_Animation(v2)/Skill/Dual_Skill05_Montage.Dual_Skill05_Montage"));
	if (skill5obj.Succeeded()) SkillNum05.DirectionMontages.Add(E4WayDirection::Default, skill5obj.Object);
	SkillMap.Add(SkillNum05.SkillName, SkillNum05);

	FSkillData SkillNum06{};
	SkillNum06.SkillName = "S_06";
	SkillNum06.Cooldown = 4.0f;
	SkillNum06.AttackPowerMultiplier = 1.5f;
	SkillNum06.Counter = true;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill6obj(TEXT("/Game/RPG_Hero_Animation(v2)/Skill/Dual_Skill06_Montage.Dual_Skill06_Montage"));
	if (skill6obj.Succeeded()) SkillNum06.DirectionMontages.Add(E4WayDirection::Default, skill6obj.Object);
	SkillMap.Add(SkillNum06.SkillName, SkillNum06);

	FSkillData SkillNum07{};
	SkillNum07.SkillName = "S_07";
	SkillNum07.Cooldown = 4.0f;
	SkillNum07.AttackPowerMultiplier = 6.4f;
	SkillNum07.StaggerAmount = 1.5f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill7obj(TEXT("/Game/RPG_Hero_Animation(v2)/Skill/Dual_Skill07_Montage.Dual_Skill07_Montage"));
	if (skill7obj.Succeeded()) SkillNum07.DirectionMontages.Add(E4WayDirection::Default, skill7obj.Object);
	SkillMap.Add(SkillNum07.SkillName, SkillNum07);

	FSkillData DownPering{};
	DownPering.SkillName = "Period";
	DownPering.Cooldown = 3.0f;
	DownPering.AttackPowerMultiplier = 0.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DownParryDefault(TEXT("/Game/RPG_Hero_Animation(v2)/Dual_Dash_Montage.Dual_Dash_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DownParryF(TEXT("/Game/RPG_Hero_Animation(v2)/Period/Dual_Period_F_Montage.Dual_Period_F_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DownParryB(TEXT("/Game/RPG_Hero_Animation(v2)/Period/Dual_Period_B_Montage.Dual_Period_B_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DownParryL(TEXT("/Game/RPG_Hero_Animation(v2)/Period/Dual_Period_L_Montage.Dual_Period_L_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DownParryR(TEXT("/Game/RPG_Hero_Animation(v2)/Period/Dual_Period_R_Montage.Dual_Period_R_Montage"));
	if (DownParryF.Succeeded()) DownPering.DirectionMontages.Add(E4WayDirection::Default, DownParryDefault.Object);
	if (DownParryF.Succeeded()) DownPering.DirectionMontages.Add(E4WayDirection::Forward, DownParryF.Object);
	if (DownParryB.Succeeded()) DownPering.DirectionMontages.Add(E4WayDirection::Back, DownParryB.Object);
	if (DownParryL.Succeeded()) DownPering.DirectionMontages.Add(E4WayDirection::Left, DownParryL.Object);
	if (DownParryR.Succeeded()) DownPering.DirectionMontages.Add(E4WayDirection::Right, DownParryR.Object);

	SkillMap.Add(DownPering.SkillName, DownPering);

	//F차징스킬(start)
	FSkillData ChargingSkill_Start{};
	ChargingSkill_Start.SkillName = "ChargingStartSkill";
	ChargingSkill_Start.Cooldown = 8.0f;
	ChargingSkill_Start.AttackPowerMultiplier = 10.0f;//스타트라서 없음 배율이
	ChargingSkill_Start.StaggerAmount = 20.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Chargingobj_S(TEXT("/Game/RPG_Hero_Animation(v2)/Skill/Dual_SKill08_02_Montage.Dual_SKill08_02_Montage"));//변경
	if (Chargingobj_S.Succeeded())
	{
		ChargingSkill_Start.SkillMontage = Chargingobj_S.Object;
	}
	SkillMap.Add(ChargingSkill_Start.SkillName, ChargingSkill_Start);
	//Dead 애니메이션
	FSkillData Dead{};
	Dead.SkillName = "Dead";
	Dead.Cooldown = 0.0f;
	Dead.AttackPowerMultiplier = 0.0f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> deadobj(TEXT("/Game/RPG_Hero_Animation(v2)/Dual_Die_Montage.Dual_Die_Montage"));
	if (deadobj.Succeeded()) Dead.DirectionMontages.Add(E4WayDirection::Default, deadobj.Object);
	SkillMap.Add(Dead.SkillName, Dead);
	//Restart애니메이션
	FSkillData Restart{};
	Restart.SkillName = "Restart";
	Restart.Cooldown = 0.0f;
	Restart.AttackPowerMultiplier = 0.0f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> restartobj(TEXT("/Game/RPG_Hero_Animation(v2)/Dual_Restart_Montage.Dual_Restart_Montage"));
	if (restartobj.Succeeded()) Restart.DirectionMontages.Add(E4WayDirection::Default, restartobj.Object);
	SkillMap.Add(Restart.SkillName, Restart);
	
	//Collision
	FSkillCollisionData Sphere_A{};
	Sphere_A.ShapeType = ESkillCollisionShapeType::Sphere;
	Sphere_A.Dimensions = FVector(200.f, 200.f, 50.f);
	Sphere_A.Duration = 0.5f;
	SkillCollisionDataArray.Add(Sphere_A);//0번

	//Collision
	FSkillCollisionData Sphere_B{};
	Sphere_B.ShapeType = ESkillCollisionShapeType::Sphere;
	Sphere_B.Dimensions = FVector(400.f, 400.f, 50.f);
	Sphere_B.Duration = 0.5f;
	SkillCollisionDataArray.Add(Sphere_B);

	FSkillCollisionData Box_A{};
	Box_A.ShapeType = ESkillCollisionShapeType::Box;
	Box_A.Dimensions = FVector(500.f, 400.f, 300.f);
	Box_A.Duration = 0.5f;
	SkillCollisionDataArray.Add(Box_A);

	FSkillCollisionData Box_B{};
	Box_B.ShapeType = ESkillCollisionShapeType::Box;
	Box_B.Dimensions = FVector(700.f, 600.f, 300.f);
	Box_B.Duration = 0.5f;
	SkillCollisionDataArray.Add(Box_B);

	FSkillCollisionData Box_C{};
	Box_C.ShapeType = ESkillCollisionShapeType::Box;
	Box_C.Dimensions = FVector(300.f, 300.f, 600.f);
	Box_C.Duration = 0.5f;
	SkillCollisionDataArray.Add(Box_C);//4번

	FSkillCollisionData Sphere_C{};
	Sphere_C.ShapeType = ESkillCollisionShapeType::Sphere;
	Sphere_C.Dimensions = FVector(600.f, 600.f, 50.f);
	Sphere_C.Duration = 0.5f;
	SkillCollisionDataArray.Add(Sphere_C);

	FSkillCollisionData Box_D{};
	Box_D.ShapeType = ESkillCollisionShapeType::Box;
	Box_D.Dimensions = FVector(800.f, 700.f, 300.f);
	Box_D.Duration = 0.5f;
	SkillCollisionDataArray.Add(Box_D);

	FSkillCollisionData Box_E{};
	Box_E.ShapeType = ESkillCollisionShapeType::Box;
	Box_E.Dimensions = FVector(300.f, 1000.f, 300.f);
	Box_E.Duration = 0.5f;
	SkillCollisionDataArray.Add(Box_E);//7번
}


void UC_SkillComponent::RequestJumpToSection(FName SectionName)
{
	if (CachedAnimInstance)
	{
		CachedAnimInstance->OnRequestJumpSection.Broadcast(SectionName);
	}
}

float UC_SkillComponent::GetskillCoolTime(FName skill_Key)
{
	if (const FSkillData* Skill = SkillMap.Find(skill_Key))
	{
		return Skill->Cooldown;
	}
	return 0.0f;
}

// Called when the game starts
void UC_SkillComponent::BeginPlay()
{
	Super::BeginPlay();
	if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
	{
		if (UC_PlayerAnimInstance* AnimInst = Cast<UC_PlayerAnimInstance>(OwnerChar->GetMesh()->GetAnimInstance()))
		{
			CachedAnimInstance = AnimInst;
		}
	}

	// ...
	
}




// Called every frame
void UC_SkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UC_SkillComponent::InitializeComponent()
{
	
}

void UC_SkillComponent::UsingSkill(FName skill_Key, E4WayDirection Direction)
{
	if (const FSkillData* Skill = SkillMap.Find(skill_Key))//스킬맵에 같은이름을가진게있다면 찿아서 Skill변수에 저장->이게 성공하면 true
	{
		UAnimMontage* MontageToPlay = nullptr;//실행할몽타주 백업용
		CurrentSkillName = skill_Key;
		// 4방향 전용 스킬이라면
		if (Skill->DirectionMontages.Num() > 0)//Skill이 방향을 가지고있는 데이터라면?
		{
			UE_LOG(LogTemp, Warning, TEXT("4wayMontage"));
			if (UAnimMontage*const* FoundMontage = Skill->DirectionMontages.Find(Direction))
			{
				MontageToPlay = *FoundMontage;
			}
			else if (UAnimMontage* const* DefaultMontage = Skill->DirectionMontages.Find(E4WayDirection::Default))
			{
				MontageToPlay = *DefaultMontage;
			}
		}
		else
		{
			// 기존 스킬 (방향 없는)
			MontageToPlay = Skill->SkillMontage;
		}
		//스킬은 존재하지만, `SkillMontage`가 설정되어 있지 않은 경우 실행 중단
		//즉, **애니메이션이 설정되지 않은 스킬은 실행하지 않음**
		if (!MontageToPlay)
		{
			UE_LOG(LogTemp, Warning, TEXT("NoMontage"));
			return;
		}
		OnSkillMontageRequested.Broadcast(MontageToPlay);//몽타주실행파트
	}
}

void UC_SkillComponent::HandleSkillHit(int32 SkillIndex, FVector SkillLocation, FRotator SkillRotation)//애님노티파이(SkillHit)호출용
{
	UE_LOG(LogTemp, Warning, TEXT("SetCollision"));
	//유효인덱스 값 검사
	if (!SkillCollisionDataArray.IsValidIndex(SkillIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid SkillIndex %d"), SkillIndex);
		return;
	}
	bool IsCounter = false;
	if (const FSkillData* Skill = SkillMap.Find(CurrentSkillName))
	{
		IsCounter = Skill->Counter;
	}
	const FSkillCollisionData& CollisionData = SkillCollisionDataArray[SkillIndex];
	SpawnSkillCollision(CollisionData, SkillLocation, SkillRotation, IsCounter);
	//const FSkillData* SkillData = SkillMap.Find(CurrentSkillName);
	//if (!SkillData)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("NonePlayingSkill"));
	//	return;
	//}
	//const FSkillCollisionData& CollisionData = SkillData->CollisionData;//스킬데이터의 컬리젼데이터를 참조하는 래퍼런스생성.
	//SpawnSkillCollision(CollisionData, SkillData->Counter);

}

bool UC_SkillComponent::IsCooldownReady(FName SkillName) const
{
	//Skill쿨타임 맵에 해당 스킬데이터가없으면 아직 눌리지않았으니 ture로 리턴
	if (!SkillCooldownEndTime.Contains(SkillName)) return true;
	return GetWorld()->GetTimeSeconds() >= SkillCooldownEndTime[SkillName];
	//현제시간 >= endTime = false면 엔드타임이 더크므로 아직 쿨타임안지남
	//현제시간 >= endTime = true면 엔드타임보다 현제시간이 지났으므로 쿨타임끝남
}

void UC_SkillComponent::StartCooldown(FName SkillName)
{
	if (!SkillMap.Contains(SkillName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill [%s] not found in SkillDataMap"), *SkillName.ToString());
		return;
	}
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("StartCooldown: GetWorld() is nullptr!"));
		return;
	}
	// 스킬 데이터에서 쿨타임 값 가져오기
	float CooldownDuration = SkillMap[SkillName].Cooldown;
	//********스킬쿨타임이 시작되는시점이 이부분 -> 여기서 델리게이트로 알려줌 스킬키를 가져와서,***********
	// 쿨타임 종료 시간 계산 후 TMap에 기록
	float EndTime = GetWorld()->GetTimeSeconds() + CooldownDuration;
	SkillCooldownEndTime.Add(SkillName, EndTime);

	// UI 동기화를 위한 브로드캐스트
	OnSkillCooldownStarted.Broadcast(SkillName);
}

float UC_SkillComponent::GetRemainingCooldown(FName SkillName) const
{
	if (!SkillCooldownEndTime.Contains(SkillName)) return 0.0f;//쿨타임리턴(눌린적이없으면)
	return FMath::Max(SkillCooldownEndTime[SkillName] - GetWorld()->GetTimeSeconds(), 0.0f);//남은쿨타임 리턴
}

void UC_SkillComponent::skillCoolTimeTriggered(FName SkillName)
{
	float Remain = GetRemainingCooldown(SkillName);
	UE_LOG(LogTemp, Warning, TEXT("Skill CoolTime = %f"), Remain);
	FS_GameAlertSubsystemConfig MessageConfig{};
	MessageConfig.fDefaultAlertDuration = 0.3f;
	MessageConfig.strDefaultAlertMessage = FText::FromString( TEXT("스킬이 쿨타임 중 입니다."));
	UC_GameAlertSubsystem::pushAlertMessage_Cpp(MessageConfig);
	//if (m_OnSkillCoolTimeTriggered.IsBound())
	//	m_OnSkillCoolTimeTriggered.Broadcast(SkillName, Remain);
}

