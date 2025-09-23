// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_BaseCharacter.h"
#include "GameFramework/Pawn.h"
#include "C_NiagaraUtil.h"
#include "C_MonsterBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMosterDied);
class UC_DamageWidgetComponent;

UENUM(BlueprintType)
enum class E_MonsterRank : uint8
{
	Normal	UMETA(DisplayName = "Normal"),
	Named	UMETA(DisplayName = "Named"),
	Boss	UMETA(DisplayName = "Boss")
};

UENUM(BlueprintType)
enum class E_MonsterPhaseState : uint8
{
	Idle,
	Attacking,
	PhaseChanging,
	PhaseChanged,
	GimmickReady,
	GimmickExecute,
	GimmickEnd,
	Stagger,
	Dead
};

UENUM(BlueprintType)
enum class E_MonsterDeferredAction : uint8
{
	None,
	PhaseChange,
	Gimmick
};

USTRUCT(BlueprintType)
struct FS_MonsterAction
{
	GENERATED_BODY()

	E_MonsterDeferredAction eType;
	float fHp;
	float fMaxHp;
};

USTRUCT(BlueprintType)
struct FS_PatternData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName strAttackName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* pAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 nWeight;

	float LastUsedTime = -100.f;
};

/**
 * 
 */
UCLASS()
class PROJECTRPG_API AC_MonsterBaseCharacter : public AC_BaseCharacter
{
	GENERATED_BODY()

private:
	/*
	* 애님몽타주
	*/

	UPROPERTY(EditAnywhere, Category = "Stagger Montage")
	UAnimMontage* m_pStaggerMontage;

	UPROPERTY(EditAnywhere, Category = "Counter Montage")
	UAnimMontage* m_pCounterMontage;

	UPROPERTY()
	class AC_MonsterAiController* m_pAiCon;

	/*
	* 컴포넌트
	*/

	UPROPERTY(EditAnywhere, Category = "Gimmick")
	class UC_StaggerGimmickComponent* m_pStaggerGimmickComp;

	UPROPERTY()
	class UC_StaggerComponent* m_pStaggerComp;

	UPROPERTY()
	class UC_CounterComponent* m_pCounterComp;

	UPROPERTY()
	class UC_PhaseComponent* m_pPhaseComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DamageWidget")
	UC_DamageWidgetComponent* m_pDamageWidget{};
private:
	TQueue<FS_MonsterAction> m_quePendingActions;

	bool m_bIsAttacking = false;

	bool m_bIsPower = false;

	bool m_bStaggerBlock = false;


	FTimerHandle m_timeHandle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster Pattern")
	TArray<FS_PatternData> m_arrPatternList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster Rank")
	E_MonsterRank m_eMonsterRank = E_MonsterRank::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster State")
	E_MonsterPhaseState m_eMonsterState = E_MonsterPhaseState::Idle;

public:
	UPROPERTY(BlueprintAssignable)
	FOnMosterDied m_onMonsterDied;

	bool bPendingPhaseChange = false;

	float m_fPendingHp = 0.f;
	float m_fPendingMaxHp = 0.f;

	bool bPendingGimmickStart = false;

	

private:
	float getDistanceToTarget() const;

	void onAttackEnd();

	



protected:
	virtual void BeginPlay() override;


public:
	AC_MonsterBaseCharacter();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void reStartAi();

	UFUNCTION(BlueprintCallable)
	void stopAi();

	UFUNCTION(BlueprintCallable)
	void tryTriggerPhaseChangeOrGimmick();

	UFUNCTION(BlueprintCallable)
	void setPhaseState(E_MonsterPhaseState eState);

	UFUNCTION(BlueprintCallable)
	E_MonsterPhaseState getCurrentState() const;

	UFUNCTION(BlueprintCallable)
	void reservePhaseChange(float fHp, float fMaxHp);

	UFUNCTION(BlueprintCallable)
	void reserveGimmick(float fHp, float fMaxHp);

	UFUNCTION(BlueprintCallable)
	void setActivePower(bool bActive);

	UFUNCTION(BlueprintCallable)
	bool getIsPower() const;

	UFUNCTION(BlueprintCallable)
	void setBlockStagger(bool bActive);

	UFUNCTION(BlueprintCallable)
	bool getIsBlockStagger() const;

	/*
	* 전투 관련
	*/

	TArray<int32> filterAvailablePatterns();

	int32 selectPatternByWeight(const TArray<int32>& arrCandidates);

	void playPattern(int32 nPatternIndex);

	bool getIsAttacking() const;


	/*
	* 무력화 관련
	*/

	void playStaggerMontage();

	UFUNCTION(BlueprintCallable)
	virtual void takeStaggerEvent(float fStagger);

	UFUNCTION()
	void onStaggerBroken();

	UFUNCTION()
	void onStaggerRecover();

	/*
	* 카운터 관련
	*/

	UFUNCTION()
	void onCounterSuccess();

	UFUNCTION()
	void onCounterFailed();

	UFUNCTION(BlueprintCallable)
	void tryCounter();

	/*
	* 죽음 소멸 관련 
	*/

	UFUNCTION(BlueprintCallable)
	void onDead();

	virtual void Destroyed() override;

	/*
	* 페이즈 관련
	*/

	UFUNCTION()
	void onPhaseChangeFinished();

	/*
	*  기믹 관련
	*/

	//void moveToGimmick();

	//void startGimmick();

	UFUNCTION()
	void playStaggerGimmick();

	UFUNCTION()
	void endStaggerGimmick();

	//VFX
	//HIT VFX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Effect")
	class UNiagaraSystem* HitVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Effect")
	class USoundBase* HitSound;

	UFUNCTION(BlueprintCallable)
	void playHitEffect();
	
};
