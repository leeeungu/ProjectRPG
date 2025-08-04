// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_StaggerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBroken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRecover);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_StaggerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	float m_fBreakTimer = 0.0f;

protected:
	UPROPERTY(EditAnywhere, Category = "Stagger")
	float m_fMaxStagger = 100.0f;
	UPROPERTY(VisibleAnywhere, Category = "Stagger")
	float m_fCurrentStagger = 0.0f;
	UPROPERTY(VisibleAnywhere, Category = "Stagger")
	float m_fBrokenDuration = 3.0f;
	UPROPERTY(VisibleAnywhere, Category = "Stagger")
	bool m_bIsBroken = false;


public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Stagger")
	FOnBroken m_onBroken;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Stagger")
	FOnRecover m_onRecover;

private:
	void recover();

public:	
	// Sets default values for this component's properties
	UC_StaggerComponent();

	UFUNCTION(BlueprintCallable, Category = "Stagger")
	void applyStagger(float fStagger);

	UFUNCTION(BlueprintCallable, Category = "Stagger")
	bool isBroken();

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
