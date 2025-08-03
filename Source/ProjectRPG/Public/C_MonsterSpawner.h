// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_MonsterSpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnComplete);

UCLASS()
class PROJECTRPG_API AC_MonsterSpawner : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	bool m_bIsActive;
	
	
public:	
	// Sets default values for this actor's properties
	AC_MonsterSpawner();

	UFUNCTION(BlueprintCallable)
	void setSpawnerActive(bool bActive);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "SpawnerDelegate")
	FOnSpawnComplete m_onSpawnComplete;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
