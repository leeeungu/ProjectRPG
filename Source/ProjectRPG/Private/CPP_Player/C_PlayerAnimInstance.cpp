// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_PlayerAnimInstance.h"
#include "CPP_Player/C_SkillComponent.h"

void UC_PlayerAnimInstance::PlaySkillMontage(UAnimMontage* MontageToPlay)
{
    if (MontageToPlay)
    {
        Montage_Play(MontageToPlay);
        UE_LOG(LogTemp, Warning, TEXT("MontagePlay"));
    }
}

void UC_PlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    APawn* OwnerPawn = TryGetPawnOwner();
    if (!OwnerPawn) return;

    UC_SkillComponent* SkillComp = OwnerPawn->FindComponentByClass<UC_SkillComponent>();//플레이어가들고있는 skillCom을 들고와서 백업본 생성
    if (SkillComp)//가져왔다면?
    {
        // 바인딩
        SkillComp->OnSkillMontageRequested.AddDynamic(this, &UC_PlayerAnimInstance::PlaySkillMontage);
        //언바인딩을해야할경우->이 애님인스턴스를 더이상 쓰지않을떄.
    }
}

void UC_PlayerAnimInstance::OnChangeRunningState()
{
    ChangeRunningState.Broadcast();
}


