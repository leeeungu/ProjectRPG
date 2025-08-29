// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_PlayerAnimInstance.h"
#include "CPP_Player/C_SkillComponent.h"

void UC_PlayerAnimInstance::PlaySkillMontage(UAnimMontage* MontageToPlay)
{
    //UE_LOG(LogTemp, Warning, TEXT("MontagePlay"));
    if (MontageToPlay)
    {
        StopAllMontages(0.1f);
        Montage_Play(MontageToPlay);
        CurrentActiveMontage = MontageToPlay;//��Ÿ�� �����
        //UE_LOG(LogTemp, Warning, TEXT("MontagePlay"));
        // ���� �̺�Ʈ ��������Ʈ: ��Ÿ�� ������ �� OnEndMontage ȣ��(�������Ʈ�� Completed ���� ��������)
        // 
        // ��¡ ��ų�̶�� Start ���Ǻ��� ���
        if (MontageToPlay->GetName().Contains("SpearSkill_08_Pull")) // Ȥ�� ActionName���� �Ǻ�
        {
            Montage_JumpToSection(FName("Start"), MontageToPlay);
            Montage_SetNextSection(FName("Start"), FName("Holding"), MontageToPlay);
            Montage_SetNextSection(FName("Holding"), FName("Holding"), MontageToPlay); // ����
        }
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
    OnRequestJumpSection.AddDynamic(this, &UC_PlayerAnimInstance::HandleJumpSection);//������ �÷��̾�� ����ȣ�������� ������Ʈ�� �Űܾ��� �׷��� �������
}

void UC_PlayerAnimInstance::OnChangeRunningState()
{
    ChangeRunningState.Broadcast();
}

void UC_PlayerAnimInstance::OnEndMontage(UAnimMontage* Montage, bool bInterrupted)
{
    //UE_LOG(LogTemp, Warning, TEXT("Montage Ended: %s"), *Montage->GetName());
    if (bInterrupted)
    {
        // ���� �ߴܵ� ��� �� �̵� ���� ���� Ǯ�� ����
        UE_LOG(LogTemp, Warning, TEXT("Montage interrupted. Ignore movement enable."));
        return;//������ �����鼭 ��ε�ĳ��Ʈ ���������������� ������ �̵��Ұ����·� �����ְԵ�.
        //���Է½ý������� �ִϸ��̼��� ���γ������ʰ� �ٸ��ִϸ��̼��� ȣ��ǵ� �̷������� �Ѿ���Ե�.
        //*�ϴ� �и��� ��ų�߰������� Interrupted�� ó�����Ǵµ� ������ �и������� bCanMove �ٽ� false�� �����Ǽ� �߰� �̵��� ���ϰԵ�.
        //�и����� ��������ǵ� ������ �и��ִϸ��̼��� ������ idle�� ���ư�
        //*�׷� ���Ͱ������¾Ƽ� �Ѿ����ԵǸ�? 
    }
    if (Montage && Montage->GetName().Contains("SpearSkill_08_Pull"))//����
    {
        UE_LOG(LogTemp, Warning, TEXT("charging end?? "));
        SetPlayerMovePointEnabled.Broadcast();
        return;
    }
    SetPlayerMovePointEnabled.Broadcast();
}

void UC_PlayerAnimInstance::HandleJumpSection(FName SectionName)
{
    if (CurrentActiveMontage)  // �ִ��ν��Ͻ����� ���� ������� ��Ÿ�� ���� �ʿ�
    {
        Montage_JumpToSection(SectionName, CurrentActiveMontage);
    }
}

void UC_PlayerAnimInstance::SetAttackMode(bool b)
{
    UE_LOG(LogTemp, Warning, TEXT("AttakcingMode_Start"));
    m_bIsAttack = b;
}

void UC_PlayerAnimInstance::SetActiveValue(bool b)
{
    if (m_bIsAttack)
    {
        m_bIsAttack = false;
        m_ActiveValue = b;
    }
    if (!m_bIsAttack)
    {
        m_bIsAttack = true;
    }
}


