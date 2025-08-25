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
	Idle,       // 입력 대기
	Busy,       // 일반 스킬/애니메이션 실행 중 (차단)
	Charging    // 차징 스킬 진행 중 (차징 입력만 허용)
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

	//플레이어 상태
	UPROPERTY()
	ERunningSystemState RunningState = ERunningSystemState::Idle;

	//이동 및 회전
	float moveSpeed = 500.0f;
	FVector moveDir{};
	float remainDist{};
	float stopDist = 0.1f;
	float rotDir{};
	float remainAngle{};
	//Path값
	TArray<FVector> pathList{};
	int curPathPos = 1;

	FVector MousePointDir{};
	bool IsPeriod = false;
	//로테이트 보간
	bool bRotate = false;               
	FQuat TargetRotationQuat;                 
	float RotateInterpSpeed = 10.0f;     

	bool bRunningSystemOpen = true;//기본값 true 항시열려있음

protected:

private:
	void CalMoveData();
	void RunningSystemManager();
	void ClearMoveState();

public:
	AC_Player();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void OnMoveToPosPlayer(FVector pos);//외부에서도 호출해서 pos값을넣어주면 해당위치로이동함.
	//void SetMousePointDir(FVector pos);
	FVector GetMousePointDir();
	void CalRotateData(const FVector& TargetPoint);
	bool IsRotating() const { return bRotate; }//로테이팅 여부확인(외부확인용)

	
	void Period();
	

	
};
