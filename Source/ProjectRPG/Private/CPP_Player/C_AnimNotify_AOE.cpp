// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_AnimNotify_AOE.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

void UC_AnimNotify_AOE::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);
    if (!MeshComp || !NiagaraEffect) return;
    UWorld* World = MeshComp->GetWorld();
    if (!World) return;

    FVector SpawnLocation;
    FRotator SpawnRotation;

    if (AOE_SocketName != NAME_None && MeshComp->DoesSocketExist(AOE_SocketName))
    {
        SpawnLocation = MeshComp->GetSocketLocation(AOE_SocketName);
        SpawnRotation = MeshComp->GetSocketRotation(AOE_SocketName);
    }
    else
    {
        SpawnLocation = MeshComp->GetComponentLocation();
        SpawnRotation = MeshComp->GetComponentRotation();
    }

    // 오프셋 적용
    SpawnLocation += AOE_RotationOffset.RotateVector(AOE_LocationOffset);
    SpawnRotation += AOE_RotationOffset;

    // 이펙트 생성
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        World,
        NiagaraEffect,
        SpawnLocation,
        SpawnRotation,
        AOE_Scale,
        true,
        true
    );
}
