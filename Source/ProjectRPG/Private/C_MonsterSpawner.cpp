// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MonsterSpawner.h"

// Sets default values
AC_MonsterSpawner::AC_MonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AC_MonsterSpawner::setSpawnerActive(bool bActive)
{
	m_bIsActive = bActive;
	SetActorTickEnabled(bActive);
}

// Called when the game starts or when spawned
void AC_MonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_MonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

