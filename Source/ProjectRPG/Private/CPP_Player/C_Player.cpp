// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player/C_Player.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AC_Player::AC_Player()
{
	//skMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMESH"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> res(TEXT("/Game/RPG_Hero_Character/Assets/Meshes/Adventurer/SK_Adventurer.SK_Adventurer"));
	if (res.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(res.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	m_springCom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRING"));
	m_springCom->SetupAttachment(RootComponent);
	m_springCom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	m_springCom->SetRelativeLocation(FVector(-450.f, 0.f, 800.f));

	m_camCom = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	m_camCom->SetupAttachment(m_springCom);
}


