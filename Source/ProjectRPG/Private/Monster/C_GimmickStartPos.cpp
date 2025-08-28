// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/C_GimmickStartPos.h"

// Sets default values
AC_GimmickStartPos::AC_GimmickStartPos()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_GimmickStartPos::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_GimmickStartPos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

