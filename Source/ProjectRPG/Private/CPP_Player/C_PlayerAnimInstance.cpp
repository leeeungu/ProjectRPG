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
        CurrentActiveMontage = MontageToPlay;//몽타주 백업용
        //UE_LOG(LogTemp, Warning, TEXT("MontagePlay"));
        // 엔진 이벤트 델리게이트: 몽타주 끝났을 때 OnEndMontage 호출(블루프린트의 Completed 노드와 같은뜻임)
        // 
        // 차징 스킬이라면 Start 섹션부터 재생
        if (MontageToPlay->GetName().Contains("SpearSkill_08_Pull")) // 혹은 ActionName으로 판별
        {
            Montage_JumpToSection(FName("Start"), MontageToPlay);
            Montage_SetNextSection(FName("Start"), FName("Holding"), MontageToPlay);
            Montage_SetNextSection(FName("Holding"), FName("Holding"), MontageToPlay); // 루프
        }
        FOnMontageEnded MontageEndedDelegate;
        MontageEndedDelegate.Unbind();//이전델리게이트해재 (중복방지)
        MontageEndedDelegate.BindUObject(this, &UC_PlayerAnimInstance::OnEndMontage);
        Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);
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
    OnRequestJumpSection.AddDynamic(this, &UC_PlayerAnimInstance::HandleJumpSection);//지금은 플레이어에서 직접호출하지만 컴포넌트로 옮겨야함 그래야 구조깔끔
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
        // 강제 중단된 경우 → 이동 가능 상태 풀지 않음
        UE_LOG(LogTemp, Warning, TEXT("Montage interrupted. Ignore movement enable."));
        return;//리턴을 때리면서 브로드캐스트 실행하지않음으로 여전히 이동불가상태로 남아있게됨.
        //선입력시스템으로 애니메이션이 전부끝나지않고 다른애니메이션이 호출되도 이로직으로 넘어오게됨.
        //*일단 패링은 스킬중간에쓰면 Interrupted로 처리가되는데 어차피 패링로직에 bCanMove 다시 false로 설정되서 추가 이동은 못하게됨.
        //패링으로 강제종료되도 어차피 패링애니메이션이 끝나면 idle로 돌아감
        //*그럼 몬스터공격을맞아서 넘어지게되면? 
    }
    if (Montage && Montage->GetName().Contains("SpearSkill_08_Pull"))//변경
    {
        UE_LOG(LogTemp, Warning, TEXT("charging end?? "));
        SetPlayerMovePointEnabled.Broadcast();
        return;
    }
    SetPlayerMovePointEnabled.Broadcast();
}

void UC_PlayerAnimInstance::HandleJumpSection(FName SectionName)
{
    if (CurrentActiveMontage)  // 애님인스턴스에서 현재 재생중인 몽타주 추적 필요
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


