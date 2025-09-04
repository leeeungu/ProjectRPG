#include "GamePlay/C_DataManager.h"

void UC_DataManager::registerDataFile(TSoftObjectPtr<IC_DataManagerInterface> pInterface)
{
	if (!pInterface)
		return;
	E_DataType Type = pInterface->getDataType();
	for (uint8 i = 1; i < 8; i++)
	{
		uint8 Bit = 1 << i;
		if (((uint8)Type & Bit) == Bit)
		{
			FString Path = pInterface->getFilePath((E_DataType)Bit);
			E_DataType* pPath = m_mapDataFile.Find(Path);
			if (!pPath)
				pPath = &m_mapDataFile.Add(Path);
			*pPath = (E_DataType)Bit;
		}
	}

	m_arrSaveObjects.AddUnique(pInterface);
}

void UC_DataManager::loadDataFiles()
{
	for (TPair<FString,  E_DataType> &  sPair : m_mapDataFile)
	{
		FString& Path = sPair.Key;
		switch (sPair.Value)
		{
		case E_DataType::E_String:
		{
			loadStringData(Path);
			break;
		}
		case E_DataType::E_StringArray:
		{
			loadStringArrayData(Path);
			break;
		}
		case E_DataType::E_Binary:
		{
			loadBinaryData(Path);
			break;
		}
		}
	}
}

void UC_DataManager::saveDataFiles()
{
	for (TSoftObjectPtr<IC_DataManagerInterface>& Obj : m_arrSaveObjects)
	{
		if (Obj.IsValid())
			Obj->saveData();
	}
	m_arrSaveObjects.Reset(0);
}



bool UC_DataManager::loadData(IC_DataManagerInterface* pInterface)
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
			bResult ^= loadData(pInterface , (E_DataType)Bit);
		}
	}
	return bResult;
}


bool UC_DataManager::loadData(IC_DataManagerInterface* pInterface, E_DataType DataType)
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
		FString* pData = m_mapStringData.Find(Path);
		if (!pData)
			return false;
		pInterface->loadStringData(*pData);
		break;
	}
	case E_DataType::E_StringArray:
	{
		TArray<FString>* pData = m_mapStringArrayData.Find(Path);
		if (!pData)
			return false;
		pInterface->loadStringArrayData(*pData);
		break;
	}
	case E_DataType::E_Binary:
	{
		TArray<uint8>* pData = m_mapBinaryArrayData.Find(Path);
		if (!pData)
			return false;
		pInterface->loadBinaryData(*pData);
		break;
	}
	}
	return true;
}

void UC_DataManager::loadStringData(const FString& FilePath)
{
	FString& Content = m_mapStringData.FindOrAdd(FilePath, FString{});
	bool bResult = FFileHelper::SaveStringToFile(Content, *FilePath);
	if (!bResult)
	{

	}
}

void UC_DataManager::loadStringArrayData(const FString& FilePath)
{
	TArray<FString>& Content = m_mapStringArrayData.FindOrAdd(FilePath, TArray<FString>{});
	bool bResult = FFileHelper::LoadFileToStringArray(Content, *FilePath);
	if (!bResult)
	{

	}
}

void UC_DataManager::loadBinaryData(const FString& FilePath)
{
	TArray<uint8>& Content = m_mapBinaryArrayData.FindOrAdd(FilePath, TArray<uint8>{});
	bool bResult = FFileHelper::LoadFileToArray(Content, *FilePath);
	if (!bResult)
	{

	}
}
