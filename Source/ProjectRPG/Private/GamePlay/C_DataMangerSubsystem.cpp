// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/C_DataMangerSubsystem.h"


void UC_DataMangerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UGameInstanceSubsystem::Initialize(Collection);
}

void UC_DataMangerSubsystem::Deinitialize()
{
	UGameInstanceSubsystem::Deinitialize();
}

bool UC_DataMangerSubsystem::loadData(IC_DataManagerInterface* pInterface)
{
	if (!pInterface)
		return false;

	bool bResult{};
	E_DataType Type = pInterface->getDataType();
	for (uint8 i = 1; i < 8; i++)
	{
		uint8 Bit = 1 << i;
		if (((uint8)Type & Bit) == Bit)
		{
			bResult &= loadData(pInterface, (E_DataType)Bit);
		}
	}
	return bResult;
}

bool UC_DataMangerSubsystem::loadData(IC_DataManagerInterface* pInterface, E_DataType DataType)
{
	if (!pInterface)
		return false;
	E_DataType Type = pInterface->getDataType();

	if (((uint8)DataType & (uint8)Type) != (uint8)Type)
		return false;

	FString Path = pInterface->getFilePath(DataType);
	switch (DataType)
	{
	case E_DataType::E_String:
	{
		FString pData{};
		if (loadStringData(Path, pData))
			pInterface->loadStringData(pData);
		break;
	}
	case E_DataType::E_StringArray:
	{
		TArray<FString> pData{};
		if (loadStringArrayData(Path, pData))
			pInterface->loadStringArrayData(pData);
		break;
	}
	case E_DataType::E_Binary:
	{
		TArray<uint8> pData{};
		if (loadBinaryData(Path, pData))
			pInterface->loadBinaryData(pData);
		break;
	}
	}
	return true;
}

bool UC_DataMangerSubsystem::saveStringData(IC_DataManagerInterface* pInterface)
{
	if (!pInterface)
		return false;
	return FFileHelper::SaveStringToFile(pInterface->getStringData(), *pInterface->getFilePath(E_DataType::E_String));
}

bool UC_DataMangerSubsystem::saveStringArrayData(IC_DataManagerInterface* pInterface)
{
	if (!pInterface)
		return false;
	return FFileHelper::SaveStringArrayToFile(pInterface->getStringArrayData(), *pInterface->getFilePath(E_DataType::E_StringArray));
}

bool UC_DataMangerSubsystem::saveBinaryData(IC_DataManagerInterface* pInterface)
{
	if (!pInterface)
		return false;
	return FFileHelper::SaveArrayToFile(pInterface->getBinaryData(), *pInterface->getFilePath(E_DataType::E_Binary));
}

bool UC_DataMangerSubsystem::loadStringData(const FString& FilePath, FString& rResult)
{
	rResult = "NONE";
	return FFileHelper::SaveStringToFile(rResult, *FilePath);
}

bool UC_DataMangerSubsystem::loadStringArrayData(const FString& FilePath, TArray<FString>& rResult)
{
	rResult.Init(FString{}, 0);
	return FFileHelper::LoadFileToStringArray(rResult, *FilePath);
}

bool UC_DataMangerSubsystem::loadBinaryData(const FString& FilePath, TArray<uint8>& rResult)
{
	rResult.Init(uint8{}, 0);
	bool bResult = FFileHelper::LoadFileToArray(rResult, *FilePath);
	return FFileHelper::LoadFileToArray(rResult, *FilePath);
}


