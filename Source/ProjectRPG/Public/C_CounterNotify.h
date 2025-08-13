// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "C_CounterNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_CounterNotify : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float m_fCanCounterDuration = 0.5f;

public:

	virtual void Notify(USkeletalMeshComponent* skMeshCom, UAnimSequenceBase* Animation) override;
	
};
