// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_StaggerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBroken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRecover);

enum class E_StaggerMode
{
	Normal,
	Gimmick
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_StaggerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	float m_fBreakTimer = 0.0f;

	E_StaggerMode m_eCurrentMode = E_StaggerMode::Normal;

	UPROPERTY(EditAnywhere, Category = "Stagger")
	float m_fMaxStagger = 100.0f;
	UPROPERTY(VisibleAnywhere, Category = "Stagger")
	float m_fCurrentStagger = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Stagger")
	float m_fBrokenDuration = 8.0f;
	UPROPERTY(VisibleAnywhere, Category = "Stagger")
	bool m_bIsBroken = false;


public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Stagger")
	FOnBroken m_onBroken;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Stagger")
	FOnRecover m_onRecover;

private:
	void recover();

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UC_StaggerComponent();

	UFUNCTION(BlueprintCallable, Category = "Stagger")
	void applyStagger(float fStagger);

	UFUNCTION(BlueprintCallable, Category = "Stagger")
	bool isBroken();

	void setMaxStaggerPoint(float fStagger);
	void setStaggerPoint(float fStagger);
	void setBreakDuration(float fDuration);

	UFUNCTION(BlueprintCallable)
	float getMaxStaggerPoint() const;
	UFUNCTION(BlueprintCallable)
	float getCurrentStaggerPoint() const;
	float getCurrentBreakPoint() const;

	void setMode(E_StaggerMode eMode);
	E_StaggerMode getStaggerMode() const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
