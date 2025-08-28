// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_SkillComponent.h"
#include "CPP_Player/S_SkillData.h"

// Sets default values for this component's properties
UC_SkillComponent::UC_SkillComponent()
{
	//Q스킬
	PrimaryComponentTick.bCanEverTick = true;
	FSkillData SkillNum01;
	SkillNum01.SkillName = "S_01";
	SkillNum01.Cooldown = 5.0f;
	SkillNum01.AttackPowerMultiplier = 200.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill1obj(TEXT("/Game/RPG_Hero_Animation/SpearSkill_01_Montage.SpearSkill_01_Montage"));
	if (skill1obj.Succeeded())
	{
		SkillNum01.SkillMontage = skill1obj.Object;
	}
	SkillMap.Add(SkillNum01.SkillName, SkillNum01);//map배열0번에 key는 skill_01임 즉 이 이름으로 Testskill1에접근가능
	//패링
	FSkillData Pering;
	Pering.SkillName = "Period";
	Pering.Cooldown = 5.0f;
	Pering.AttackPowerMultiplier = 10.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Peringobj(TEXT("/Game/RPG_Hero_Animation/SpearPeriod_Top/SpearPeriod_T_F_Montage.SpearPeriod_T_F_Montage"));
	if (skill1obj.Succeeded())
	{
		Pering.SkillMontage = Peringobj.Object;
	}
	SkillMap.Add(Pering.SkillName, Pering);//map배열0번에 key는 skill_01임 즉 이 이름으로 Testskill1에접근가능

	//F차징스킬(start)
	FSkillData ChargingSkill_Start;
	ChargingSkill_Start.SkillName = "ChargingStartSkill";
	ChargingSkill_Start.Cooldown = 10.0f;
	ChargingSkill_Start.AttackPowerMultiplier = 0.f;//스타트라서 없음 배율이
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Chargingobj_S(TEXT("/Game/RPG_Hero_Animation/SpearSkill_08_Start.SpearSkill_08_Start"));
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


// Called when the game starts
void UC_SkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}




// Called every frame
void UC_SkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UC_SkillComponent::UsingSkill(FName skill_Key)
{
	if (const FSkillData* Skill = SkillMap.Find(skill_Key))//스킬맵에 같은이름을가진게있다면 찿아서 Skill변수에 저장->이게 성공하면 true
	{
		//스킬은 존재하지만, `SkillMontage`가 설정되어 있지 않은 경우 실행 중단
		//즉, **애니메이션이 설정되지 않은 스킬은 실행하지 않음**
		if (!Skill->SkillMontage)
		{
			UE_LOG(LogTemp, Warning, TEXT("NoMontage"));
			return;
		}
		OnSkillMontageRequested.Broadcast(Skill->SkillMontage);//몽타주실행파트
	}
}

