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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	E_MonsterRank m_eMonsterRank = E_MonsterRank::Normal;

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
	
};
