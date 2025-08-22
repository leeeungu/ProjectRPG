// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BaseCharacter.h"

// Sets default values
AC_BaseCharacter::AC_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

bool AC_BaseCharacter::takeDamageEvent_Implementation(float fDamage)
{

	m_fHp -= fDamage;


	if (m_fHp > 0)
		return true;

	if (m_fHp <= 0)
		m_onDead.Broadcast();


	return false;
	
}

void AC_BaseCharacter::setHp(float fHp)
{
	brodcastStatChange(m_onChangeHP,m_fHp, fHp);
	m_fHp = fHp;
}

float AC_BaseCharacter::getHp() const
{
	return m_fHp;
}

void AC_BaseCharacter::setMaxHp(float fMaxHp)
{
	brodcastStatChange(m_onChangeMaxHP, m_fMaxHp, fMaxHp);
	m_fMaxHp = fMaxHp;
}

float AC_BaseCharacter::getMaxHp() const
{
	return m_fMaxHp;
}

void AC_BaseCharacter::setAtk(float fAtk)
{
	brodcastStatChange(m_onChangeAtk, m_fAtk, fAtk);
	m_fAtk = fAtk;
}

float AC_BaseCharacter::getAtk() const
{
	return m_fAtk;
}

bool AC_BaseCharacter::getIsDead() const
{
	if (m_fHp <= 0)
		return true;

	return false;
}

void AC_BaseCharacter::brodcastStatChange(FOnStatChange& Delegate, float PreValue, float CurValue)
{
	if (Delegate.IsBound())
	{
		Delegate.Broadcast(PreValue ,CurValue);
	}

}
