// Fill out your copyright notice in the Description page of Project Settings.


#include "C_TestGitAI.h"

C_TestGitAI::C_TestGitAI()
{
	int a{};
	for (int i = 0; i < 10; i++)
	{
		a += i;
	}
	if (a > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("C_TestGitAI initialized with a positive sum: %d"), a);
	}
}

C_TestGitAI::~C_TestGitAI()
{
}
