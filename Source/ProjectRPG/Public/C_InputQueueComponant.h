// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_InputQueueComponant.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_InputQueueComponant : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TArray<int32> InputQueue;

public:	
	// Sets default values for this component's properties
	UC_InputQueueComponant();
	UFUNCTION(BlueprintCallable, Category = "InputQueue")
	void InputQueueList(int32 InputKeyData, bool InputIsPossible);

	UFUNCTION(BlueprintCallable, Category = "InputQueue")
	int32 OutputLastIndex() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
