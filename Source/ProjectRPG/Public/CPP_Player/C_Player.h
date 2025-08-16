// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_BaseCharacter.h"
#include "C_Player.generated.h"


class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class PROJECTRPG_API AC_Player : public AC_BaseCharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* m_springCom{};
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* m_camCom{};

protected:

public:
	AC_Player();

	
};
