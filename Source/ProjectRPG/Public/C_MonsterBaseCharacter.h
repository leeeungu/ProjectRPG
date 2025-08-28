// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_BaseCharacter.h"
#include "GameFramework/Pawn.h"
#include "C_NiagaraUtil.h"
#include "C_MonsterBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMosterDied);

UENUM(BlueprintType)
enum class E_MonsterRank : uint8
{
	Normal	UMETA(DisplayName = "Normal"),
	Named	UMETA(DisplayName = "Named"),
	Boss	UMETA(DisplayName = "Boss")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* pNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fNiagaraLife;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fNiagaraScale;

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
	UPROPERTY(EditAnywhere, Category = "Stagger Montage")
	UAnimMontage* m_pStaggerMontage;

	UPROPERTY(EditAnywhere, Category = "Gimmick")
	class UC_StaggerGimmickComponent* m_pStaggerGimmickComp;

	UPROPERTY()
	class UC_StaggerComponent* m_pStaggerComp;

	UPROPERTY()
	class UC_PhaseComponent* m_pPhaseComp;

	UPROPERTY()
	class UC_CounterComponent* m_pCounterComp;

	bool m_bIsAttacking = false;


	FTimerHandle m_timeHandle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster Pattern")
	TArray<FS_PatternData> m_arrPatternList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster Rank")
	E_MonsterRank m_eMonsterRank = E_MonsterRank::Normal;

	

private:
	float getDistanceToTarget() const;

	void onAttackEnd();


protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnMosterDied m_onMonsterDied;

public:
	AC_MonsterBaseCharacter();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void stopAi();

	UFUNCTION()
	void onMontageEnded_moveToGimmick(UAnimMontage* Montage, bool bInterrupted);

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

	/*
	* 죽음 소멸 관련 
	*/

	UFUNCTION(BlueprintCallable)
	void onDead();

	virtual void Destroyed() override;

	/*
	*  기믹 관련
	*/
	FVector getGimmickPos();

	void moveToGimmick();

	void startGimmick();

	UFUNCTION()
	void playStaggerGimmick();


	UFUNCTION()
	void endStaggerGimmick();

	

	
};
