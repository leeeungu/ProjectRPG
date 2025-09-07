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
    mySkillComp->HandleSkillHit();
}
