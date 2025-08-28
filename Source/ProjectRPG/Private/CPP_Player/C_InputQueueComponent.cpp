// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_InputQueueComponent.h"
#include "CPP_Player/S_InputActionData.h"

// Sets default values for this component's properties
UC_InputQueueComponent::UC_InputQueueComponent() : UActorComponent{}
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//SetActive(true);
	//PrimaryComponentTick.bStartWithTickEnabled = false;
	//SetComponentTickEnabled(false);
	InputQueue.Reserve(MaxQueueSize);
	ChargingQueue.Reserve(MaxQueueSize);
	// ...
}

void UC_InputQueueComponent::PushInput(const FInputActionData& NewInput)
{
	if (NewInput.InputType == EInputType::None) return;
	if (InputQueue.Num() >= MaxQueueSize)
	{
		InputQueue.RemoveAt(0);
	}
	//���� �������� �ð��� ���
	FInputActionData TimedInput = NewInput;
	TimedInput.Timestamp = GetWorld()->GetTimeSeconds();
	InputQueue.Add(TimedInput);
	SetComponentTickEnabled(true);
	//InputQueue.Add(NewInput);
}

void UC_InputQueueComponent::PushInput_Charging(const FInputActionData& NewInput)
{
	if (NewInput.InputType == EInputType::None) return;
	if (InputQueue.Num() >= MaxQueueSize)
	{
		InputQueue.RemoveAt(0);
	}
	ChargingQueue.Add(NewInput);
	SetComponentTickEnabled(true);

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

bool UC_InputQueueComponent::GetLastChargingInputData(FInputActionData& OutInput) const
{
	if (ChargingQueue.Num() > 0)
	{
		OutInput = ChargingQueue.Last();
		return true;
	}
	return false;
}

void UC_InputQueueComponent::ClearQueueList()
{
	InputQueue.Empty();
}

void UC_InputQueueComponent::ClearChargingQueueList()
{
	ChargingQueue.Empty();
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
	UE_LOG(LogTemp, Warning, TEXT("Tick True"));
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float ExpireTime = 0.3f;
	// �տ������� ������ �Է� ����
	while (InputQueue.Num() > 0)
	{
		if (CurrentTime - InputQueue[0].Timestamp >= ExpireTime)
		{
			InputQueue.RemoveAt(0);
		}
		else
		{
			break; // ���ĵ� �����̹Ƿ� �ڴ� Ȯ���� �ʿ� ����
		}
	}
	// ť�� ������� ƽ ������
	if (InputQueue.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tick false"));
		SetComponentTickEnabled(false);
	}
	

	// ...
}



