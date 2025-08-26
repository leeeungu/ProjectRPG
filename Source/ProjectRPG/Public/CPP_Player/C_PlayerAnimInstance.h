// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_PlayerAnimInstance.generated.h"
DECLARE_MULTICAST_DELEGATE(FChangeRunningStateDelegate);
DECLARE_MULTICAST_DELEGATE(FSetPlayerMovePointEnabled);//player의 bCanMove 활성화->MoveToPos사용가능상태로 변경

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
	FSetPlayerMovePointEnabled SetPlayerMovePointEnabled;
	void OnEndMontage(UAnimMontage* Montage, bool bInterrupted);//end몽타주 바인딩용 함수(매개변수는 맞춰준것뿐)

};
