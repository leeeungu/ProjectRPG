// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_BaseCharacter.h"
#include "C_MonsterAiController.h"
#include "AIController.h"
#include "C_StaggerComponent.h"
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
struct FS_AttackData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString strAttackName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* pAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fAttackRange;
	bool bIsCool = false;
};
/**
 * 
 */
UCLASS()
class PROJECTRPG_API AC_MonsterBaseCharacter : public AC_BaseCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Attack Montage")
	UAnimMontage* m_pAttackMontage;

	UC_StaggerComponent* m_pStaggerComp;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TArray<FS_AttackData> m_arrAttackList;

	int m_nCurrentAttackIndex;

	FTimerHandle m_timeHandle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	E_MonsterRank m_eMonsterRank = E_MonsterRank::Normal;

private:
	void startAttackCoolTime(FS_AttackData& sAttackData);
	void resetAttackCoolTime();

protected:
	virtual void BeginPlay() override;

public:
	AC_MonsterBaseCharacter();

	void playAttackMontage();

	UFUNCTION(BlueprintCallable)
	virtual void takeStaggerEvent(float fStagger);

	UFUNCTION()
	void onStaggerBroken();

	UFUNCTION()
	void onStaggerRecover();

	float getAttackRange() const;
	
};
