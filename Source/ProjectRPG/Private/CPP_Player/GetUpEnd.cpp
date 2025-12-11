// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/GetUpEnd.h"
#include "CPP_Player/C_PlayerAnimInstance.h"

void UGetUpEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);
    if (MeshComp == nullptr) return;

    UC_PlayerAnimInstance* AnimInstance = Cast<UC_PlayerAnimInstance>(MeshComp->GetAnimInstance());
    if (AnimInstance)
    {
        AnimInstance->OnChangeDownState(); // 애님 인스턴스 내 함수 호출
    }
}
