// Fill out your copyright notice in the Description page of Project Settings.


#include "C_CounterComponent.h"

void UC_CounterComponent::endCounterWindow()
{
	m_bCanCounter = false;
}

// Sets default values for this component's properties
UC_CounterComponent::UC_CounterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UC_CounterComponent::startCounterWindow(float fDuration)
{
	m_bCanCounter = true;

	GetWorld()->GetTimerManager().ClearTimer(m_sCounterHandle);
	GetWorld()->GetTimerManager().SetTimer(m_sCounterHandle, this,
		&UC_CounterComponent::endCounterWindow,fDuration, false);
}

bool UC_CounterComponent::tryCounter(AActor* pPlayer)
{
	if (!m_bCanCounter)
	{
		m_onCounterFailed.Broadcast();
		return false;
	}

	m_bCanCounter = false;

	m_onCounterSuccess.Broadcast();
	return true;
}


// Called when the game starts
void UC_CounterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UC_CounterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

