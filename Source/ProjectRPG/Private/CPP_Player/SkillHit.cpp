// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/SkillHit.h"
#include "CPP_Player/C_Player.h"
#include "CPP_Player/C_SkillComponent.h"

void USkillHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);
    if (MeshComp == nullptr) return;
    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;
    AC_Player* myPlayer = Cast<AC_Player>(Owner);
    if (!myPlayer) return;
    UC_SkillComponent* mySkillComp = myPlayer->FindComponentByClass<UC_SkillComponent>();
    if (!mySkillComp) return;

    FVector SocketLocation = MeshComp->GetComponentLocation();
    FRotator SocketRotation = FRotator::ZeroRotator;

    if (SocketName != NAME_None && MeshComp->DoesSocketExist(SocketName))
    {
        SocketLocation = MeshComp->GetSocketLocation(SocketName);
        SocketRotation = MeshComp->GetSocketRotation(SocketName);
    }

    // 소켓 위치에 오프셋 더하기 (로컬 좌표계 오프셋이므로 월드 변환 필요)
    FVector FinalLocation = SocketLocation + SocketRotation.RotateVector(CollisionOffset);
    FRotator FinalRotation = (SocketRotation + CollisionRotationOffset).GetNormalized();
    
    mySkillComp->HandleSkillHit(CollisionIndex, FinalLocation, FinalRotation);
}
