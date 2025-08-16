// Fill out your copyright notice in the Description page of Project Settings.


#include "C_InputQueueComponant.h"

// Sets default values for this component's properties
UC_InputQueueComponant::UC_InputQueueComponant()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UC_InputQueueComponant::InputQueueList(int32 InputKeyData, bool InputIsPossible)
{
	const int32 MaxSize = 5;
	if (InputQueue.Num() >= MaxSize)
	{
		InputQueue.RemoveAt(0);
	}
	if (InputIsPossible)
	{
		if (bSubQueueActive)//bool check
		{
			if (EnterFirst.Num() >= 2)
			{
				EnterFirst.RemoveAt(0);
			}
			EnterFirst.Add(InputKeyData);
		}
		InputQueue.Add(InputKeyData);
	}
	else
	{
		InputQueue.Add(0);
	}
}

int32 UC_InputQueueComponant::OutputLastIndex(bool IsPull) const
{
	if (IsPull)
	{
		if (InputQueue.Num() == 0)
		{
			return 0;
		}
		return InputQueue.Last();
	}
	else
		return 0;
	
}

void UC_InputQueueComponant::ClearList()
{
	InputQueue.Empty();
}

bool UC_InputQueueComponant::InputTimeOver(float delta)//*Improve
{
	float AllClearTime{};
	AllClearTime += delta;
	if (AllClearTime > 2.f)
	{
		return true;
	}
	else
		return false;
}


// Called when the game starts
void UC_InputQueueComponant::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UC_InputQueueComponant::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

