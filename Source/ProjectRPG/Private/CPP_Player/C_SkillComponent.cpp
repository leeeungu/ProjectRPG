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
	SkillMap.Add(Testskill1.SkillName, Testskill1);//map배열0번에 key는 skill_01임 즉 이 이름으로 Testskill1에접근가능
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
	if (const FSkillData* Skill = SkillMap.Find(skillName))//스킬맵에 같은이름을가진게있다면 찿아서 Skill변수에 저장->이게 성공하면 true
	{
		//-스킬은 존재하지만, `SkillMontage`가 설정되어 있지 않은 경우 실행 중단
		//- 즉, ** 애니메이션이 설정되지 않은 스킬은 실행하지 않음**
		if (!Skill->SkillMontage) return;

		/*-캐릭터의 `Mesh`에서 애니메이션 인스턴스를 가져옴
		- 이 인스턴스는 현재 캐릭터의 애니메이션을 제어하는 주체*/
		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		if (!OwnerCharacter) return;


		/*UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(Skill->SkillMontage);
		}*/
	}
}

