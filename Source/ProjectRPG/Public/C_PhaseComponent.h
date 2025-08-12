// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_PhaseComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPhaseChange);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_PhaseComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	int32 m_nCurrentPhaseIndex = -1;

	float m_fChangePercentHp = 70.f;


public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Phase")
	FOnPhaseChange m_onPhaseChange;

	UPROPERTY(EditAnywhere, Category = "Phase");
	TArray<float> m_arrPhase;
	

public:	
	// Sets default values for this component's properties
	UC_PhaseComponent();

	UFUNCTION(BlueprintCallable)
	void phaseChange(float fHp);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
