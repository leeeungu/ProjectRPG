// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_BaseCharacter.h"
#include "GameFramework/Pawn.h"  
#include "C_MonsterBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMosterDied, AC_MonsterBaseCharacter*, DeadMonster);

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

	UPROPERTY()
	class UC_StaggerComponent* m_pStaggerComp;

	UPROPERTY()
	class UC_PhaseComponent* m_pPhaseComp;

	UPROPERTY()
	class UC_CounterComponent* m_pCounterComp;


	bool m_bIsAttacking = false;

	

	FTimerHandle m_timeHandle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	TArray<FS_PatternData> m_arrPatternList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	E_MonsterRank m_eMonsterRank = E_MonsterRank::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	FName m_strMonsterName;

public:
	UPROPERTY(BlueprintAssignable)
	FOnMosterDied m_onMonsterDied;

private:
	float getDistanceToTarget() const;

	void onAttackEnd();


protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;


public:
	AC_MonsterBaseCharacter();

	virtual void Tick(float DeltaTime) override;

	TArray<int32> filterAvailablePatterns();

	int32 selectPatternByWeight(const TArray<int32>& arrCandidates);

	void playPattern(int32 nPatternIndex);

	void playStaggerMontage();

	bool getIsAttacking() const;


	UFUNCTION(BlueprintCallable)
	virtual void takeStaggerEvent(float fStagger);

	UFUNCTION(BlueprintCallable)
	void stopAi();




	/*
	* 무력화 관련
	*/

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
	void onDead();

};
