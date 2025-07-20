// Fill out your copyright notice in the Description page of Project Settings.


#include "C_TestGitAI.h"

C_TestGitAI::C_TestGitAI()
{
	int* pNode = new int(5);
	pNode = new int(5);
}

C_TestGitAI::~C_TestGitAI()
{
	delete pNode; // This will cause a memory leak if not handled properly
	pNode = nullptr; // Reset pointer to avoid dangling pointer
}
