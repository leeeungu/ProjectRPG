// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_InputQueueComponent.h"
#include "CPP_Player/S_InputActionData.h"

// Sets default values for this component's properties
UC_InputQueueComponent::UC_InputQueueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	InputQueue.Reserve(MaxQueueSize);
	// ...
}

void UC_InputQueueComponent::PushInput(const FInputActionData& NewInput)
{
	if (NewInput.InputType == EInputType::None) return;
	if (InputQueue.Num() >= MaxQueueSize)
	{
		InputQueue.RemoveAt(0);
	}
	InputQueue.Add(NewInput);
}

bool UC_InputQueueComponent::GetLastInputData(FInputActionData& OutInput) const
{
	if (InputQueue.Num() > 0)
	{
		OutInput = InputQueue.Last();
		return true;
	}
	return false;
}

void UC_InputQueueComponent::ClearQueueList()
{
	InputQueue.Empty();
}


// Called when the game starts
void UC_InputQueueComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UC_InputQueueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	// ...
}



