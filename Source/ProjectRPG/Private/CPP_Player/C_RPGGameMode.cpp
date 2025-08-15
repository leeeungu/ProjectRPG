// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_RPGGameMode.h"
#include "CPP_Player/C_Player.h"
#include "CPP_Player/C_PlayerController.h"

AC_RPGGameMode::AC_RPGGameMode()
{
	DefaultPawnClass = AC_Player::StaticClass();
	PlayerControllerClass = AC_PlayerController::StaticClass();
}
