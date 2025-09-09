// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "I_PlayerToAnimInstance.h"
#include "C_PlayerAnimInstance.generated.h"
DECLARE_MULTICAST_DELEGATE(FChangeRunningStateDelegate);
DECLARE_MULTICAST_DELEGATE(FChangeDownState);
DECLARE_MULTICAST_DELEGATE(FSetPlayerMovePointEnabled);//player의 bCanMove 활성화->MoveToPos사용가능상태로 변경
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestJumpSection, FName, SectionName);//플레이어가 애님인스턴스로 주는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponModeChanged, bool, bIsWeaponEquipped);//Unequip노티파이로부터 bool값호출받음

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_PlayerAnimInstance : public UAnimInstance ,public II_PlayerToAnimInstance
{
	
	GENERATED_BODY()
private:
	UAnimMontage* CurrentActiveMontage{};
	void AnimChangeMode();
	UFUNCTION()
	void OnUnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	UPROPERTY(BlueprintReadOnly)
	bool m_bIsAttackMode = false;
	UPROPERTY(BlueprintReadOnly)
	bool m_bIsAttack = false;
	UPROPERTY(BlueprintReadOnly)
	bool m_ActiveValue = false;
	UPROPERTY(BlueprintReadOnly)
	bool m_IsDown = false;
	UPROPERTY()
	UAnimMontage* UnEquipMontage;
	UPROPERTY()
	UAnimMontage* DownMontage;
public:
	virtual void NativeInitializeAnimation() override;
	UPROPERTY(BlueprintReadOnly)
	bool IsMove{};
	UFUNCTION()
	void PlaySkillMontage(UAnimMontage* MontageToPlay);
	UFUNCTION()
	void PlayUnEquipMontage();
	UFUNCTION()
	void PlayDownMontage();
	//노티파이발생시 사용함수
	FChangeRunningStateDelegate ChangeRunningState;
	void OnChangeRunningState();
	FChangeDownState ChangeDownState;
	void OnChangeDownState();
	FSetPlayerMovePointEnabled SetPlayerMovePointEnabled;
	void OnEndMontage(UAnimMontage* Montage, bool bInterrupted);//end몽타주 바인딩용 함수(매개변수는 맞춰준것뿐)
	

	UPROPERTY(BlueprintAssignable, Category = "Montage")
	FOnRequestJumpSection OnRequestJumpSection;
	UFUNCTION()
	void HandleJumpSection(FName SectionName);

	UPROPERTY(BlueprintAssignable)
	FOnWeaponModeChanged OnWeaponModeChanged;

	//인터페이스
	virtual void SetAttackMode(bool b) override;
	virtual void SetIsAttackingMode(bool b) override;
	virtual void SetIsDownMode(bool b) override;

};
