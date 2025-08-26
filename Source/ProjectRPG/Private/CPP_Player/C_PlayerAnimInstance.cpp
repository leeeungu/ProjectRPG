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

    UC_SkillComponent* SkillComp = OwnerPawn->FindComponentByClass<UC_SkillComponent>();//�÷��̾����ִ� skillCom�� ���ͼ� ����� ����
    if (SkillComp)//�����Դٸ�?
    {
        // ���ε�
        SkillComp->OnSkillMontageRequested.AddDynamic(this, &UC_PlayerAnimInstance::PlaySkillMontage);
        //����ε����ؾ��Ұ��->�� �ִ��ν��Ͻ��� ���̻� ����������.
    }
}

void UC_PlayerAnimInstance::OnChangeRunningState()
{
    ChangeRunningState.Broadcast();
}


