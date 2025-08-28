// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_PlayerAnimInstance.generated.h"
DECLARE_MULTICAST_DELEGATE(FChangeRunningStateDelegate);
DECLARE_MULTICAST_DELEGATE(FSetPlayerMovePointEnabled);//player�� bCanMove Ȱ��ȭ->MoveToPos��밡�ɻ��·� ����
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChargingReadyChanged, bool);//�÷��̾��� bIsReady�� ����->Charging������ �÷��׸� �����Ŵ���� ��������
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestJumpSection, FName, SectionName);//�÷��̾ �ִ��ν��Ͻ��� �ִ� ��������Ʈ

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_PlayerAnimInstance : public UAnimInstance
{
	
	GENERATED_BODY()
private:
	UAnimMontage* CurrentActiveMontage{};
public:
	virtual void NativeInitializeAnimation() override;
	UPROPERTY(BlueprintReadOnly)
	bool IsMove{};
	UFUNCTION()
	void PlaySkillMontage(UAnimMontage* MontageToPlay);
	//��Ƽ���̹߻��� ����Լ�
	FChangeRunningStateDelegate ChangeRunningState;
	void OnChangeRunningState();
	FSetPlayerMovePointEnabled SetPlayerMovePointEnabled;
	void OnEndMontage(UAnimMontage* Montage, bool bInterrupted);//end��Ÿ�� ���ε��� �Լ�(�Ű������� �����ذͻ�)
	FOnChargingReadyChanged ChargingReadyChanged;

	UPROPERTY(BlueprintAssignable, Category = "Montage")
	FOnRequestJumpSection OnRequestJumpSection;
	UFUNCTION()
	void HandleJumpSection(FName SectionName);

};
