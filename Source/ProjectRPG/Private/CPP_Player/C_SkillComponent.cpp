// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_SkillComponent.h"
#include "CPP_Player/S_SkillData.h"
#include "CPP_Player/C_PlayerAnimInstance.h"
#include "GameFramework/Character.h"

void UC_SkillComponent::SpawnSkillCollision(const FSkillCollisionData& data)
{
	FVector SpawnLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorRotation().RotateVector(data.RelativeOffset);
	FQuat Rotation = GetOwner()->GetActorQuat();

	FCollisionShape CollisionShape;
	switch (data.ShapeType)
	{
	case ESkillCollisionShapeType::Sphere:
		CollisionShape = FCollisionShape::MakeSphere(data.Dimensions.X);
		break;
	case ESkillCollisionShapeType::Box:
		CollisionShape = FCollisionShape::MakeBox(data.Dimensions);
		break;
	case ESkillCollisionShapeType::Capsule:
		CollisionShape = FCollisionShape::MakeCapsule(data.Dimensions.X, data.Dimensions.Z);
		break;
	}

	// 데미지 적용 로직 예시
	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		SpawnLocation,
		SpawnLocation,
		Rotation,
		ECC_Pawn,
		CollisionShape
	);

#ifdef DEBUG_DRAW
	switch (data.ShapeType)
	{
	case ESkillCollisionShapeType::Sphere:
		DrawDebugSphere(GetWorld(), SpawnLocation, data.Dimensions.X, 16, FColor::Green, false, 2.f);
		break;

	case ESkillCollisionShapeType::Box:
		DrawDebugBox(GetWorld(), SpawnLocation, data.Dimensions * 0.5f, Rotation, FColor::Blue, false, 2.f);
		break;

	case ESkillCollisionShapeType::Capsule:
		DrawDebugCapsule(GetWorld(), SpawnLocation, data.Dimensions.Z, data.Dimensions.X, Rotation, FColor::Red, false, 2.f);
		break;
	}
#endif
	//블록?, 오버랩? 
	//if (bHit)
	//{
	//	for (const FHitResult& Hit : HitResults)
	//	{
	//		AActor* HitActor = Hit.GetActor();
	//		if (HitActor && HitActor != GetOwner())
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitActor->GetName());
	//          //takedamge호출, 넘겨줄 데미지 계산해서 담아서 보냄.
	//          //함수가 호출되면 이 데미지를 매개변수로 브로드캐스트해서 몬스터의 receive함수호출
	//          //receive가 불리면 바인딩된 자체 함수로 들어가서 데미지를 HP로부터 깍음.
	// 
	// //여기서 카운터스킬은 몬스터의 트라이카운터호출까지 해야됨.
	//그럼 스킬중에 카운터스킬에는 카운터스킬이 있다는것을 설정해줘야함.
	// //노티파이에서 카운터 실행 (몬스터베이스에서 트라이카운터를 가져오는게 목표)
	//		}
	//	}
	//}
}

// Sets default values for this component's properties
UC_SkillComponent::UC_SkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//평타
	FSkillData PlainAttack01;
	PlainAttack01.SkillName = "PA_01";
	PlainAttack01.Cooldown = 0.0f;//테스트용 0초
	PlainAttack01.AttackPowerMultiplier = 200.f;
	PlainAttack01.CollisionData.ShapeType = ESkillCollisionShapeType::Sphere;
	PlainAttack01.CollisionData.Dimensions = FVector(100.f, 100.f, 100.f);
	PlainAttack01.CollisionData.RelativeOffset = FVector::ForwardVector * 50.f;
	PlainAttack01.CollisionData.Duration = 2.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PA1obj(TEXT("/Game/RPG_Hero_Animation/SpearComboAttack/Spear_Combo04_1_Montage.Spear_Combo04_1_Montage"));
	if (PA1obj.Succeeded()) PlainAttack01.DirectionMontages.Add(E4WayDirection::Default, PA1obj.Object);
	SkillMap.Add(PlainAttack01.SkillName, PlainAttack01);

	FSkillData PlainAttack02;
	PlainAttack02.SkillName = "PA_02";
	PlainAttack02.Cooldown = 0.0f;//테스트용 0초
	PlainAttack02.AttackPowerMultiplier = 200.f;
	PlainAttack02.CollisionData.ShapeType = ESkillCollisionShapeType::Box;
	PlainAttack02.CollisionData.Dimensions = FVector(70.f, 70.f, 200.f);
	PlainAttack02.CollisionData.RelativeOffset = FVector::ForwardVector * 60.f;
	PlainAttack02.CollisionData.Duration = 2.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PA2obj(TEXT("/Game/RPG_Hero_Animation/SpearComboAttack/Spear_Combo04_2_Montage.Spear_Combo04_2_Montage"));
	if (PA1obj.Succeeded()) PlainAttack02.DirectionMontages.Add(E4WayDirection::Default, PA2obj.Object);
	SkillMap.Add(PlainAttack02.SkillName, PlainAttack02);

	FSkillData PlainAttack03;
	PlainAttack03.SkillName = "PA_03";
	PlainAttack03.Cooldown = 0.0f;//테스트용 0초
	PlainAttack03.AttackPowerMultiplier = 200.f;
	PlainAttack03.CollisionData.ShapeType = ESkillCollisionShapeType::Box;
	PlainAttack03.CollisionData.Dimensions = FVector(300.f, 100.f, 100.f);
	PlainAttack03.CollisionData.RelativeOffset = FVector::ForwardVector * 250.f;
	PlainAttack03.CollisionData.Duration = 2.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PA3obj(TEXT("/Game/RPG_Hero_Animation/SpearComboAttack/Spear_Combo04_3_Montage.Spear_Combo04_3_Montage"));
	if (PA1obj.Succeeded()) PlainAttack03.DirectionMontages.Add(E4WayDirection::Default, PA3obj.Object);
	SkillMap.Add(PlainAttack03.SkillName, PlainAttack03);

	FSkillData PlainAttack04;
	PlainAttack04.SkillName = "PA_04";
	PlainAttack04.Cooldown = 0.0f;//테스트용 0초
	PlainAttack04.AttackPowerMultiplier = 200.f;
	PlainAttack04.CollisionData.ShapeType = ESkillCollisionShapeType::Sphere;
	PlainAttack04.CollisionData.Dimensions = FVector(100.f, 100.f, 50.f);
	PlainAttack04.CollisionData.RelativeOffset = FVector::ForwardVector * 150.f;
	PlainAttack04.CollisionData.Duration = 2.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PA4obj(TEXT("/Game/RPG_Hero_Animation/SpearComboAttack/Spear_Combo04_4_Montage.Spear_Combo04_4_Montage"));
	if (PA1obj.Succeeded()) PlainAttack04.DirectionMontages.Add(E4WayDirection::Default, PA4obj.Object);
	SkillMap.Add(PlainAttack04.SkillName, PlainAttack04);

	//Q스킬
	FSkillData SkillNum01;
	SkillNum01.SkillName = "S_01";
	SkillNum01.Cooldown = 0.0f;//테스트용 0초
	SkillNum01.AttackPowerMultiplier = 200.f;
	SkillNum01.CollisionData.ShapeType = ESkillCollisionShapeType::Box;
	SkillNum01.CollisionData.Dimensions = FVector(100.f, 100.f, 500.f);
	SkillNum01.CollisionData.RelativeOffset = FVector::ForwardVector * 250.f;
	SkillNum01.CollisionData.Duration = 2.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill1obj(TEXT("/Game/RPG_Hero_Animation/SpearSkill_01_Montage.SpearSkill_01_Montage"));
	if (skill1obj.Succeeded()) SkillNum01.DirectionMontages.Add(E4WayDirection::Default, skill1obj.Object);
	SkillMap.Add(SkillNum01.SkillName, SkillNum01);

	FSkillData SkillNum02;
	SkillNum02.SkillName = "S_02";
	SkillNum02.Cooldown = 0.0f;
	SkillNum02.AttackPowerMultiplier = 200.f;
	SkillNum02.CollisionData.ShapeType = ESkillCollisionShapeType::Sphere;
	SkillNum02.CollisionData.Dimensions = FVector(400.f, 400.f, 300.f);
	SkillNum02.CollisionData.RelativeOffset = FVector::ForwardVector * 50.f;
	SkillNum02.CollisionData.Duration = 2.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill2obj(TEXT("/Game/RPG_Hero_Animation/SpearSkill_02_Montage.SpearSkill_02_Montage"));
	if (skill2obj.Succeeded()) SkillNum02.DirectionMontages.Add(E4WayDirection::Default, skill2obj.Object);
	SkillMap.Add(SkillNum02.SkillName, SkillNum02);

	FSkillData SkillNum03;
	SkillNum03.SkillName = "S_03";
	SkillNum03.Cooldown = 5.0f;
	SkillNum03.AttackPowerMultiplier = 200.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill3obj(TEXT("/Game/RPG_Hero_Animation/SpearSkill_03_Montage.SpearSkill_03_Montage"));
	if (skill3obj.Succeeded()) SkillNum03.DirectionMontages.Add(E4WayDirection::Default, skill3obj.Object);
	SkillMap.Add(SkillNum03.SkillName, SkillNum03);

	FSkillData SkillNum04;
	SkillNum04.SkillName = "S_04";
	SkillNum04.Cooldown = 5.0f;
	SkillNum04.AttackPowerMultiplier = 200.f;
	SkillNum04.CollisionData.ShapeType = ESkillCollisionShapeType::Sphere;
	SkillNum04.CollisionData.Dimensions = FVector(250.f, 250.f, 150.f);
	SkillNum04.CollisionData.RelativeOffset = FVector::ForwardVector * 50.f;
	SkillNum04.CollisionData.Duration = 2.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill4obj(TEXT("/Game/RPG_Hero_Animation/SpearSkill_04_Montage.SpearSkill_04_Montage"));
	if (skill4obj.Succeeded()) SkillNum04.DirectionMontages.Add(E4WayDirection::Default, skill4obj.Object);
	SkillMap.Add(SkillNum04.SkillName, SkillNum04);

	FSkillData SkillNum05;
	SkillNum05.SkillName = "S_05";
	SkillNum05.Cooldown = 5.0f;
	SkillNum05.AttackPowerMultiplier = 200.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill5obj(TEXT("/Game/RPG_Hero_Animation/SpearSkill_05(Tumble)/SpearSkill_07_B_Montage.SpearSkill_07_B_Montage"));
	if (skill5obj.Succeeded()) SkillNum05.DirectionMontages.Add(E4WayDirection::Default, skill5obj.Object);
	SkillMap.Add(SkillNum05.SkillName, SkillNum05);

	FSkillData SkillNum06;
	SkillNum06.SkillName = "S_06";
	SkillNum06.Cooldown = 5.0f;
	SkillNum06.AttackPowerMultiplier = 200.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill6obj(TEXT("/Game/RPG_Hero_Animation/SpearSkill_06_Montage.SpearSkill_06_Montage"));
	if (skill6obj.Succeeded()) SkillNum06.DirectionMontages.Add(E4WayDirection::Default, skill6obj.Object);
	SkillMap.Add(SkillNum06.SkillName, SkillNum06);

	FSkillData SkillNum07;
	SkillNum07.SkillName = "S_07";
	SkillNum07.Cooldown = 0.0f;
	SkillNum07.AttackPowerMultiplier = 200.f;
	SkillNum07.CollisionData.ShapeType = ESkillCollisionShapeType::Box;
	SkillNum07.CollisionData.Dimensions = FVector(700.f, 100.f, 100.f);
	SkillNum07.CollisionData.RelativeOffset = FVector::ForwardVector * 250.f;
	SkillNum07.CollisionData.Duration = 0.3f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill7obj(TEXT("/Game/RPG_Hero_Animation/SpearSkill_07_Montage.SpearSkill_07_Montage"));
	if (skill7obj.Succeeded()) SkillNum07.DirectionMontages.Add(E4WayDirection::Default, skill7obj.Object);
	SkillMap.Add(SkillNum07.SkillName, SkillNum07);

	//패링
	//FSkillData Pering;
	//Pering.SkillName = "1Period";
	//Pering.Cooldown = 5.0f;
	//Pering.AttackPowerMultiplier = 0.f;
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> Peringobj(TEXT("/Game/RPG_Hero_Animation/SpearPeriod_Top/SpearPeriod_T_F_Montage.SpearPeriod_T_F_Montage"));
	//if (skill1obj.Succeeded())
	//{
	//	Pering.SkillMontage = Peringobj.Object;
	//}
	//SkillMap.Add(Pering.SkillName, Pering);//map배열0번에 key는 skill_01임 즉 이 이름으로 Testskill1에접근가능
	//다운패링
	FSkillData DownPering;
	DownPering.SkillName = "Period";
	DownPering.Cooldown = 5.0f;
	DownPering.AttackPowerMultiplier = 0.f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DownParryDefault(TEXT("/Game/RPG_Hero_Animation/SpearPeriod_Top/SpearPeriod_T_F_Montage.SpearPeriod_T_F_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DownParryF(TEXT("/Game/RPG_Hero_Animation/SpearPeriod_Down/SpearPeriod_D_F_Montage.SpearPeriod_D_F_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DownParryB(TEXT("/Game/RPG_Hero_Animation/SpearPeriod_Down/SpearPeriod_D_B_Montage.SpearPeriod_D_B_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DownParryL(TEXT("/Game/RPG_Hero_Animation/SpearPeriod_Down/SpearPeriod_D_L_Montage.SpearPeriod_D_L_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DownParryR(TEXT("/Game/RPG_Hero_Animation/SpearPeriod_Down/SpearPeriod_D_R_Montage.SpearPeriod_D_R_Montage"));
	if (DownParryF.Succeeded()) DownPering.DirectionMontages.Add(E4WayDirection::Default, DownParryDefault.Object);
	if (DownParryF.Succeeded()) DownPering.DirectionMontages.Add(E4WayDirection::Forward, DownParryF.Object);
	if (DownParryB.Succeeded()) DownPering.DirectionMontages.Add(E4WayDirection::Back, DownParryB.Object);
	if (DownParryL.Succeeded()) DownPering.DirectionMontages.Add(E4WayDirection::Left, DownParryL.Object);
	if (DownParryR.Succeeded()) DownPering.DirectionMontages.Add(E4WayDirection::Right, DownParryR.Object);

	SkillMap.Add(DownPering.SkillName, DownPering);

	//F차징스킬(start)
	FSkillData ChargingSkill_Start;
	ChargingSkill_Start.SkillName = "ChargingStartSkill";
	ChargingSkill_Start.Cooldown = 0.0f;
	ChargingSkill_Start.AttackPowerMultiplier = 0.f;//스타트라서 없음 배율이
	ChargingSkill_Start.CollisionData.ShapeType = ESkillCollisionShapeType::Box;
	ChargingSkill_Start.CollisionData.Dimensions = FVector(800.f, 100.f, 50.f);
	ChargingSkill_Start.CollisionData.RelativeOffset = FVector::ForwardVector * 300.f;
	ChargingSkill_Start.CollisionData.Duration = 2.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Chargingobj_S(TEXT("/Game/RPG_Hero_Animation/SpearSkill_08_Pull.SpearSkill_08_Pull"));//변경
	if (Chargingobj_S.Succeeded())
	{
		ChargingSkill_Start.SkillMontage = Chargingobj_S.Object;
	}
	SkillMap.Add(ChargingSkill_Start.SkillName, ChargingSkill_Start);
	//F차징스킬(Hold)
	FSkillData ChargingSkill_Hold;
	ChargingSkill_Hold.SkillName = "ChargingHoldingSkill";
	ChargingSkill_Hold.Cooldown = 0.0f;
	ChargingSkill_Hold.AttackPowerMultiplier = 0.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Chargingobj_H(TEXT("/Game/RPG_Hero_Animation/SpearSkill_08_Hold.SpearSkill_08_Hold"));
	if (Chargingobj_H.Succeeded())
	{
		ChargingSkill_Hold.SkillMontage = Chargingobj_H.Object;
	}
	SkillMap.Add(ChargingSkill_Hold.SkillName, ChargingSkill_Hold);
	//F차징스킬(Cancel)
	FSkillData ChargingSkill_Cancel;
	ChargingSkill_Cancel.SkillName = "ChargingEndCancelSkill";
	ChargingSkill_Cancel.Cooldown = 0.0f;
	ChargingSkill_Cancel.AttackPowerMultiplier = 100.f;//막타 + 실패 배율
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Chargingobj_CC(TEXT("/Game/RPG_Hero_Animation/SpearSkill_08_EndCancel.SpearSkill_08_EndCancel"));
	if (Chargingobj_CC.Succeeded())
	{
		ChargingSkill_Cancel.SkillMontage = Chargingobj_CC.Object;
	}
	SkillMap.Add(ChargingSkill_Cancel.SkillName, ChargingSkill_Cancel);
	//F차징스킬(Complete)
	FSkillData ChargingSkill_Complete;
	ChargingSkill_Complete.SkillName = "ChargingEndCompleteSkill";
	ChargingSkill_Complete.Cooldown = 0.0f;
	ChargingSkill_Complete.AttackPowerMultiplier = 200.f;//막타 + 성공 배율
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Chargingobj_CP(TEXT("/Game/RPG_Hero_Animation/SpearSkill_08_EndComplete.SpearSkill_08_EndComplete"));
	if (Chargingobj_CP.Succeeded())
	{
		ChargingSkill_Complete.SkillMontage = Chargingobj_CP.Object;
	}
	SkillMap.Add(ChargingSkill_Complete.SkillName, ChargingSkill_Complete);
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

void UC_SkillComponent::HandleSkillHit()//애님노티파이(SkillHit)호출용
{
	UE_LOG(LogTemp, Warning, TEXT("SetCollision"));
	const FSkillData* SkillData = SkillMap.Find(CurrentSkillName);
	if (!SkillData)
	{
		UE_LOG(LogTemp, Warning, TEXT("NonePlayingSkill"));
		return;
	}
	const FSkillCollisionData& CollisionData = SkillData->CollisionData;//스킬데이터의 컬리젼데이터를 참조하는 래퍼런스생성.
	SpawnSkillCollision(CollisionData);

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

