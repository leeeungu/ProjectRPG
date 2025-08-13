// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_CounterComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCounterSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCounterFailed);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_CounterComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	bool m_bCanCounter;
	FTimerHandle m_sCounterHandle;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Counter")
	FOnCounterSuccess m_onCounterSuccess;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Counter")
	FOnCounterFailed m_onCounterFailed;

private:
	void endCounterWindow();

public:	
	// Sets default values for this component's properties
	UC_CounterComponent();

	void startCounterWindow(float fDuration);

	UFUNCTION(BlueprintCallable, Category = "Counter")
	bool tryCounter(AActor* pPlayer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
