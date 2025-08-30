// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/UnEquipMontageEnd.h"
#include "CPP_Player/C_PlayerAnimInstance.h"

void UUnEquipMontageEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);
    if (MeshComp == nullptr) return;

    UC_PlayerAnimInstance* AnimInstance = Cast<UC_PlayerAnimInstance>(MeshComp->GetAnimInstance());
    if (AnimInstance)
    {
        II_PlayerToAnimInstance* AnimInterface = Cast<II_PlayerToAnimInstance>(AnimInstance);
        if (AnimInterface)
        {
            AnimInterface->SetActiveValue(false);
            
            UE_LOG(LogTemp, Warning, TEXT("Period true"));
        }
    }
}
