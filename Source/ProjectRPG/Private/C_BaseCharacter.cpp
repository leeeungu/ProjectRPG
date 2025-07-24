// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BaseCharacter.h"

// Sets default values
AC_BaseCharacter::AC_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_nHp = m_nMaxHp;

}

// Called when the game starts or when spawned
void AC_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AC_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AC_BaseCharacter::setHp(int nHp)
{
	m_nHp = nHp;
}

int AC_BaseCharacter::getHp()
{
	return m_nHp;
}

