// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_SkillComponent.h"
#include "CPP_Player/S_SkillData.h"

// Sets default values for this component's properties
UC_SkillComponent::UC_SkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	FSkillData Testskill1;
	Testskill1.SkillName = "Skill_01";
	Testskill1.Cooldown = 5.0f;
	Testskill1.AttackPowerMultiplier = 200.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> skill1obj(TEXT("/Game/RPG_Hero_Animation/SpearSkill_01_Montage.SpearSkill_01_Montage_C"));
	if (skill1obj.Succeeded())
	{
		Testskill1.SkillMontage = skill1obj.Object;
	}
	SkillMap.Add(Testskill1.SkillName, Testskill1);//map�迭0���� key�� skill_01�� �� �� �̸����� Testskill1�����ٰ���
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

void UC_SkillComponent::skill1(FName skillName)
{
	if (const FSkillData* Skill = SkillMap.Find(skillName))//��ų�ʿ� �����̸����������ִٸ� �O�Ƽ� Skill������ ����->�̰� �����ϸ� true
	{
		//-��ų�� ����������, `SkillMontage`�� �����Ǿ� ���� ���� ��� ���� �ߴ�
		//- ��, ** �ִϸ��̼��� �������� ���� ��ų�� �������� ����**
		if (!Skill->SkillMontage) return;

		/*-ĳ������ `Mesh`���� �ִϸ��̼� �ν��Ͻ��� ������
		- �� �ν��Ͻ��� ���� ĳ������ �ִϸ��̼��� �����ϴ� ��ü*/
		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		if (!OwnerCharacter) return;


		/*UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(Skill->SkillMontage);
		}*/
	}
}

