// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify_PlayNiagaraEffect.h"
#include "C_MonsterGimmickPlayNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_MonsterGimmickPlayNotify : public UAnimNotify_PlayNiagaraEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* m_pNiagara{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector m_sRelativePosition{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName m_SocketName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector m_vScale{};
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
