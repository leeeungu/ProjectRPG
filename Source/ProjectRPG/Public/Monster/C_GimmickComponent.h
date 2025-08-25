// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_MonsterBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "C_MonsterAiController.h"
#include "C_GimmickComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_GimmickComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	bool m_bGimmickPlaying = false;

protected:
	float m_fTriggerHp = 100.f;
	float m_fGimmickTime = 100.f;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AC_MonsterBaseCharacter> m_pMonster;
	UPROPERTY(BlueprintReadOnly)
	UBlackboardComponent* m_pBBcom;


public:	
	// Sets default values for this component's properties
	UC_GimmickComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual bool canGimmickStart();

	virtual void excuteGimmick();

	virtual bool endGimmick();

	void updateGimmickBool();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
