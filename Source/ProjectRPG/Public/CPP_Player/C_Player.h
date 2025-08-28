// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_BaseCharacter.h"
#include "C_Player.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UC_InputQueueComponent;
class UC_SkillComponent;
UENUM()
enum class ERunningSystemState : uint8
{
	Idle,       // �Է� ���
	Down,       // �ٿ�
	Busy,       // �Ϲ� ��ų/�ִϸ��̼� ���� �� (����)
	Charging    // ��¡ ��ų ���� �� (��¡ �Է¸� ���)
};
enum class E4WayDirection : uint8
{
	Foward,
	Back,
	Left,
	Right
};
/**
 * 
 */
UCLASS()
class PROJECTRPG_API AC_Player : public AC_BaseCharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* m_springCom{};
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* m_camCom{};
	UPROPERTY(VisibleAnywhere)
	UC_InputQueueComponent* m_inputQueue{};
	UPROPERTY(VisibleAnywhere)
	UC_SkillComponent* m_skillCom{};

	//�÷��̾� ����
	UPROPERTY()
	ERunningSystemState RunningState = ERunningSystemState::Idle;

	//�̵� �� ȸ��
	float moveSpeed = 500.0f;
	FVector moveDir{};
	float remainDist{};
	float stopDist = 0.1f;
	float rotDir{};
	float remainAngle{};
	bool bCanMove = true;//���콺������ �̵�����? 
	//Path��
	TArray<FVector> pathList{};
	int curPathPos = 1;
	//�и�
	FVector MousePointDir{};
	bool IsPeriod = false;
	float PeriodDist = 300.f;
	FVector ParryDirection;
	//������Ʈ ����
	bool bRotate = false; //true�� ƽ�� �����Լ� ����(�����ϰ����ϴ� ��������ġ �ʿ�)          
	FQuat TargetRotationQuat;                 
	float RotateInterpSpeed = 20.0f;     

	//��¡��ų
	bool bHoldSkillPlayed = false;
	bool bChargingReady = false;
public:
	void HandleChargingReady(bool bIsReady);

protected:
	UFUNCTION()
	void HandleChangeRunningState();
private:
	void CalMoveData();
	void RunningSystemManager();
	void ClearMoveState();
	void Set4_WayDirection(const FVector& mousePoint);

public:
	AC_Player();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void OnMoveToPosPlayer(FVector pos);//�ܺο����� ȣ���ؼ� pos�����־��ָ� �ش���ġ���̵���.
	FVector GetMousePointDir();
	void CalRotateData(const FVector& TargetPoint);
	bool IsRotating() const { return bRotate; }//�������� ����Ȯ��(�ܺ�Ȯ�ο�)
	void SetCanMove() { bCanMove = true; }
	//�÷��̾� canmove�� Ʈ��� �ٲ���(moveToOnPos Ȱ��ȭ) ��Ÿ�ְ��������� ȣ���
	//�߰��� ��Ÿ�ְ� ���Է����� ������ ������ �ٸ���Ÿ�ַ� �����Ǹ� �׳� return�������� �ش��Լ�ȣ���̾ȵ�.

	UFUNCTION(BlueprintCallable, Category = "RunningSystem")
	void SetRunningSystemState(ERunningSystemState newState) { RunningState = newState; }//���׽�����Ʈ ����
	UFUNCTION(BlueprintCallable, Category = "RunningSystem")
	ERunningSystemState GetRunningSystemState() { return RunningState; }


	
	void SetPeriodInfo();
	
	

	
};
