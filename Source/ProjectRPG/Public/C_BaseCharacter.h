// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);

UCLASS()
class PROJECTRPG_API AC_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_BaseCharacter();

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Dead")
	FOnDead m_onDead;
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "AC_BaseCharacter")
	float m_fMaxHp = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly ,Category = "AC_BaseCharacter")
	float m_fHp;
	UPROPERTY(EditAnywhere, Category = "AC_BaseCharacter")
	float m_fAtk;

private:
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool takeDamageEvent(float fDamage);

	UFUNCTION(BlueprintCallable)
	void setHp(float fHp);
	UFUNCTION(BlueprintPure)
	float getHp() const;

	UFUNCTION(BlueprintCallable)
	void setMaxHp(float fMaxHp);
	UFUNCTION(BlueprintPure)
	float getMaxHp() const;

	UFUNCTION(BlueprintCallable)
	void setAtk(float fAtk);
	UFUNCTION(BlueprintPure)
	float getAtk() const;

	UFUNCTION(BlueprintCallable)
	bool getIsDead() const;

};
