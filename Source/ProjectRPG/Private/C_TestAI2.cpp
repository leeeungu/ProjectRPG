// Fill out your copyright notice in the Description page of Project Settings.


#include "C_TestAI2.h"

C_TestAI2::C_TestAI2()
{
	int a{};
	for (int i = 0; i < 100; i++)
	{
		if (true)
			a += i;
	}

	for (int i = 0; i < 100; i++)
	{
		if (true)
			a += i;
	}
	UE_LOG(LogTemp, Warning, TEXT("C_TestAI2 created with a value of %d"), a);

}

C_TestAI2::~C_TestAI2()
{
}
