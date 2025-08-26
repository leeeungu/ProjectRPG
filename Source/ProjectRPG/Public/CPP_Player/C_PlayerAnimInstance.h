// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_PlayerAnimInstance.generated.h"
DECLARE_MULTICAST_DELEGATE(FChangeRunningStateDelegate);

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	UPROPERTY(BlueprintReadOnly)
	bool IsMove{};
	UFUNCTION()
	void PlaySkillMontage(UAnimMontage* MontageToPlay);
	//노티파이발생시 사용함수
	FChangeRunningStateDelegate ChangeRunningState;
	void OnChangeRunningState();

};
