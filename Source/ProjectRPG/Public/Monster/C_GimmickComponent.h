// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_GimmickComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_GimmickComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	

protected:
	bool m_bGimmickPlaying = false;
	bool m_bSuccessGimmick = false;
	float m_fTriggerHp = 0.f;
	float m_fGimmickTime = 0.f;



public:	
	// Sets default values for this component's properties
	UC_GimmickComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void endGimmick();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual bool canGimmickStart(float fHp, float fMaxHp);

	virtual void excuteGimmick();

	float getGimmickTime() const;

	bool IsPlayingGimmick();
		
};
