// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_BaseCharacter.h"
#include "C_Player.generated.h"


class USpringArmComponent;
class UCameraComponent;
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

	//�̵� �� ȸ��
	float moveSpeed = 500.0f;
	FVector moveDir{};
	float remainDist{};
	float stopDist = 0.1f;
	float rotDir{};
	float remainAngle{};
	//Path��
	TArray<FVector> pathList{};
	int curPathPos = 1;
	void CalMoveData();

	FVector MousePointDir{};
	bool IsPeriod = false;
	//������Ʈ ����
	bool bRotate = false;               
	FQuat TargetRotationQuat;                 
	float RotateInterpSpeed = 10.0f;          

protected:

public:
	AC_Player();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void OnMoveToPosPlayer(FVector pos);
	//void SetMousePointDir(FVector pos);
	FVector GetMousePointDir();
	void ClearMoveState();
	void CalRotateData(const FVector& TargetPoint);
	bool IsRotating() const { return bRotate; }//�������� ����Ȯ��(�ܺ�Ȯ�ο�)
	void Period();
	

	
};
