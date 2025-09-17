// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_GimmickComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_GimmickComponent : public UActorComponent
{
	GENERATED_BODY()


protected:
	bool m_bGimmickPlaying = false;
	bool m_bSuccessGimmick = false;
	float m_fTriggerHp = 0.f;
	float m_fGimmickTime = 0.f;

	UPROPERTY()
	class AC_MonsterBaseCharacter* m_pMonster;

	UPROPERTY()
	UAnimInstance* m_pAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Gimmick | Animation")
	UAnimMontage* m_pGimmickStartMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gimmick | Animation")
	UAnimMontage* m_pGimmikPlayMontage;

	UPROPERTY()
	FVector m_vGimmickPos;

public:	
	// Sets default values for this component's properties
	UC_GimmickComponent();

private:
	bool isMonsterIdle() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void playMontageWithCallBack(UAnimMontage* pMontage, FName strFunctionName);

	virtual void endGimmick();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual bool canGimmickStart(float fHp, float fMaxHp);

	UFUNCTION(BlueprintCallable)
	virtual void startGimmick();

	virtual void excuteGimmick();

	UFUNCTION()
	virtual void onStartGimmickMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	float getGimmickTime() const;

	bool canPlayGimmickMontage() const;

	bool IsPlayingGimmick();
		
};
