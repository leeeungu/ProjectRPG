// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_PhaseComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPhaseChange);

class AC_MonsterBaseCharacter;


USTRUCT(BlueprintType)
struct FS_PhaseData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fChangePercentHp = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* pPhaseMontage = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_PhaseComponent : public UActorComponent
{
	GENERATED_BODY()	

private:
	int32 m_nCurrentPhaseIndex = 0;

	UPROPERTY()
	AC_MonsterBaseCharacter* m_pMonster;

	UPROPERTY()
	UAnimInstance* m_pAnim;


public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Phase")
	FOnPhaseChange m_onPhaseChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	TArray<FS_PhaseData> m_arrPhase;
	

public:	
	// Sets default values for this component's properties
	UC_PhaseComponent();

	UFUNCTION(BlueprintCallable)
	void phaseChange(float fHp);

	UFUNCTION()
	void OnMontageEnded_PhaseChange(UAnimMontage* Montage, bool bInterrupted);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
