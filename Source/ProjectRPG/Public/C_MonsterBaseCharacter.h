// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_BaseCharacter.h"
#include "C_MonsterAiController.h"
#include "AIController.h"
#include "C_StaggerComponent.h"
#include "C_PhaseComponent.h"
#include "GameFramework/Pawn.h"  
#include "C_MonsterBaseCharacter.generated.h"

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
	float fMinHpPercent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fMaxHpPercent;

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
	UC_StaggerComponent* m_pStaggerComp;

	UPROPERTY()
	UC_PhaseComponent* m_pPhaseComp;


	bool m_bIsAttacking = false;

	

	FTimerHandle m_timeHandle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	TArray<FS_PatternData> m_arrPatternList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	E_MonsterRank m_eMonsterRank = E_MonsterRank::Normal;

private:
	float getDistanceToTarget() const;

	void onAttackEnd();


protected:
	virtual void BeginPlay() override;


public:
	AC_MonsterBaseCharacter();

	virtual void Tick(float DeltaTime) override;

	TArray<int32> filterAvailablePatterns();

	int32 selectPatternByWeight(const TArray<int32>& arrCandidates);

	void playPattern(int32 nPatternIndex);

	void playStaggerMontage();


	UFUNCTION(BlueprintCallable)
	virtual void takeStaggerEvent(float fStagger);

	UFUNCTION(BlueprintCallable)
	void stopAi();

	UFUNCTION()
	void onStaggerBroken();

	UFUNCTION()
	void onStaggerRecover();

	UFUNCTION(BlueprintCallable)
	void onDead();

};
