// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_MonsterSpawnerManager.generated.h"

UCLASS()
class PROJECTRPG_API AC_MonsterSpawnerManager : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TArray<AC_MonsterSpawner*> m_arrFoundSpawner;
	UPROPERTY()
	int32 m_nCurrentIndex;
	
public:	
	// Sets default values for this actor's properties
	AC_MonsterSpawnerManager();

	UFUNCTION()
	void onCurrentSpawnerComplete();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void activateCurrentSpawner();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
