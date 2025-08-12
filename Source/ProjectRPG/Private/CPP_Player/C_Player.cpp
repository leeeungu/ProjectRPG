// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_Player.h"

AC_Player::AC_Player()
{
	skMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMESH"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> res(TEXT("/Game/RPG_Hero_Character/Assets/Meshes/Adventurer/SK_Adventurer.SK_Adventurer"));
	if (res.Succeeded())
	{
		skMesh->SetSkeletalMesh(res.Object);
	}
	skMesh->SetupAttachment(RootComponent);
	skMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
}
