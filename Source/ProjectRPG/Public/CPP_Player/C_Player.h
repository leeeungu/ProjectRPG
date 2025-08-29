// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_BaseCharacter.h"
#include "Interface/C_CameraInterface.h"
#include "I_PlayerToAnimInstance.h"
#include "C_Player.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UC_InputQueueComponent;
class UC_SkillComponent;
class USceneCaptureComponent2D;
class UC_InteractionDetectorComponent;
class UC_TravelManagerComponent;

UENUM()
enum class ERunningSystemState : uint8
{
	Idle,       // 입력 대기
	Down,       // 다운
	Busy,       // 일반 스킬/애니메이션 실행 중 (차단)
	Charging    // 차징 스킬 진행 중 (차징 입력만 허용)
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
class PROJECTRPG_API AC_Player : public AC_BaseCharacter, public IC_CameraInterface
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

	UPROPERTY()
	TScriptInterface<II_PlayerToAnimInstance> myAnimInterface;
	
	UPROPERTY(VisibleAnywhere, Category = "PlayerInfoCaptureComponent", meta = (DisplayName = "PlayerInfoCaptureComponent"), BlueprintGetter = getPlayerInfoCaptureComponent)
	USceneCaptureComponent2D* m_pPlayerInfoCaptureComponent{};
	UPROPERTY(VisibleAnywhere, Category = "InteractionDetectComponent", meta = (DisplayName = "InteractionDetectComponent"), BlueprintGetter = getInteractionDetectComponent)
	UC_InteractionDetectorComponent* m_pInteractionDetectComponent{};
	UPROPERTY(VisibleAnywhere, Category = "TravelComponent", meta = (DisplayName = "TravelComponent"), BlueprintGetter = getTravelComponent)
	UC_TravelManagerComponent* m_pTravelComponent{};


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
	bool bCanMove = true;//마우스포인터 이동가능? 
	//Path값
	TArray<FVector> pathList{};
	int curPathPos = 1;
	//패링
	FVector MousePointDir{};
	bool IsPeriod = false;
	float PeriodDist = 300.f;
	FVector ParryDirection;
	//로테이트 보간
	bool bRotate = false; //true시 틱의 보간함수 실행(보간하고자하는 포인팅위치 필요)          
	FQuat TargetRotationQuat;                 
	float RotateInterpSpeed = 20.0f;     

	//차징스킬
	bool bHoldSkillPlayed = false;
	bool bChargingReady = false;
public:
	

protected:
	UFUNCTION()
	void HandleChangeRunningState();
private:
	void CalMoveData();
	void RunningSystemManager();
	void ClearMoveState();
	void Set4_WayDirection(const FVector& mousePoint);

	//플레이어 상태(idle? attck?)
	bool IsAttackMode = false;
	float AttackingModeTime = 0.f;
	void AttackMode();
	void PlayerStateCheking(float DeltaTime);

public:
	AC_Player();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void OnMoveToPosPlayer(FVector pos);//외부에서도 호출해서 pos값을넣어주면 해당위치로이동함.
	FVector GetMousePointDir();
	void CalRotateData(const FVector& TargetPoint);
	bool IsRotating() const { return bRotate; }//로테이팅 여부확인(외부확인용)
	void SetCanMove() { bCanMove = true; }
	//플레이어 canmove값 트루로 바꿔줌(moveToOnPos 활성화) 몽타주가끝나야지 호출됨
	//중간에 몽타주가 선입력으로 끝까지 못가고 다른몽타주로 블랜딩되면 그냥 return시켜져서 해당함수호출이안됨.

	UFUNCTION(BlueprintCallable, Category = "RunningSystem")
	void SetRunningSystemState(ERunningSystemState newState) { RunningState = newState; }//러닝스테이트 세팅
	UFUNCTION(BlueprintCallable, Category = "RunningSystem")
	ERunningSystemState GetRunningSystemState() { return RunningState; }


	virtual UCameraComponent* getCamera_Implementation() override;
	virtual void Reset_Implementation(UCameraComponent* Camera) override;
	
	void SetPeriodInfo();

	UFUNCTION(BlueprintPure, Category = "PlayerInfoCaptureComponent")
	USceneCaptureComponent2D*  getPlayerInfoCaptureComponent() { return m_pPlayerInfoCaptureComponent; }
	UFUNCTION(BlueprintPure, Category = "InteractionDetectComponent")
	UC_InteractionDetectorComponent* getInteractionDetectComponent() { return m_pInteractionDetectComponent; }
	UFUNCTION(BlueprintPure, Category = "TravelComponent")
	UC_TravelManagerComponent* getTravelComponent() { return m_pTravelComponent ; }
};
