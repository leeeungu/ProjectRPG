// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_PlayerAnimInstance.h"
#include "CPP_Player/C_SkillComponent.h"

void UC_PlayerAnimInstance::PlaySkillMontage(UAnimMontage* MontageToPlay)
{
    if (MontageToPlay)
    {
        StopAllMontages(0.1f);
        Montage_Play(MontageToPlay);
        UE_LOG(LogTemp, Warning, TEXT("MontagePlay"));
        // ���� �̺�Ʈ ��������Ʈ: ��Ÿ�� ������ �� OnEndMontage ȣ��(�������Ʈ�� Completed ���� ��������)
        FOnMontageEnded MontageEndedDelegate;
        MontageEndedDelegate.Unbind();//������������Ʈ���� (�ߺ�����)
        MontageEndedDelegate.BindUObject(this, &UC_PlayerAnimInstance::OnEndMontage);
        Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);
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

void UC_PlayerAnimInstance::OnEndMontage(UAnimMontage* Montage, bool bInterrupted)
{
    if (bInterrupted)
    {
        // ���� �ߴܵ� ��� �� �̵� ���� ���� Ǯ�� ����
        UE_LOG(LogTemp, Warning, TEXT("Montage interrupted. Ignore movement enable."));
        return;//������ �����鼭 ��ε�ĳ��Ʈ ���������������� ������ �̵��Ұ����·� �����ְԵ�.
        //���Է½ý������� �ִϸ��̼��� ���γ������ʰ� �ٸ��ִϸ��̼��� ȣ��ǵ� �̷������� �Ѿ���Ե�.
        //*�ϴ� �и��� ��ų�߰������� Interrupted�� ó�����Ǵµ� ������ �и������� bCanMove �ٽ� false�� �����Ǽ� �߰� �̵��� ���ϰԵ�.
    }
    SetPlayerMovePointEnabled.Broadcast();
}


