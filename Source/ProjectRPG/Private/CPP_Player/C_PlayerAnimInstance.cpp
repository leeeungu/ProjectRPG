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
        // 엔진 이벤트 델리게이트: 몽타주 끝났을 때 OnEndMontage 호출(블루프린트의 Completed 노드와 같은뜻임)
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
}

void UC_PlayerAnimInstance::OnChangeRunningState()
{
    ChangeRunningState.Broadcast();
}

void UC_PlayerAnimInstance::OnEndMontage(UAnimMontage* Montage, bool bInterrupted)
{
    if (bInterrupted)
    {
        // 강제 중단된 경우 → 이동 가능 상태 풀지 않음
        UE_LOG(LogTemp, Warning, TEXT("Montage interrupted. Ignore movement enable."));
        return;//리턴을 때리면서 브로드캐스트 실행하지않음으로 여전히 이동불가상태로 남아있게됨.
        //선입력시스템으로 애니메이션이 전부끝나지않고 다른애니메이션이 호출되도 이로직으로 넘어오게됨.
        //*일단 패링은 스킬중간에쓰면 Interrupted로 처리가되는데 어차피 패링로직에 bCanMove 다시 false로 설정되서 추가 이동은 못하게됨.
    }
    SetPlayerMovePointEnabled.Broadcast();
}


