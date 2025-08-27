// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_SkillComponent.h"
#include "CPP_Player/S_SkillData.h"

// Sets default values for this component's properties
UC_SkillComponent::UC_SkillComponent()
{
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
	SkillMap.Add(SkillNum01.SkillName, SkillNum01);//map�迭0���� key�� skill_01�� �� �� �̸����� Testskill1�����ٰ���
	//�и�
	FSkillData Pering;
	Pering.SkillName = "Period";
	Pering.Cooldown = 5.0f;
	Pering.AttackPowerMultiplier = 0.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Peringobj(TEXT("/Game/RPG_Hero_Animation/SpearPeriod_Top/SpearPeriod_T_F_Montage.SpearPeriod_T_F_Montage"));
	if (skill1obj.Succeeded())
	{
		Pering.SkillMontage = Peringobj.Object;
	}
	SkillMap.Add(Pering.SkillName, Pering);//map�迭0���� key�� skill_01�� �� �� �̸����� Testskill1�����ٰ���
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
	if (const FSkillData* Skill = SkillMap.Find(skill_Key))//��ų�ʿ� �����̸����������ִٸ� �O�Ƽ� Skill������ ����->�̰� �����ϸ� true
	{
		//��ų�� ����������, `SkillMontage`�� �����Ǿ� ���� ���� ��� ���� �ߴ�
		//��, **�ִϸ��̼��� �������� ���� ��ų�� �������� ����**
		if (!Skill->SkillMontage)
		{
			UE_LOG(LogTemp, Warning, TEXT("NoMontage"));
			return;
		}
		OnSkillMontageRequested.Broadcast(Skill->SkillMontage);//��Ÿ�ֽ�����Ʈ
	}
}

