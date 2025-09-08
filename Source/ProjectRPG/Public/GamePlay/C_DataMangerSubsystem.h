// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "C_DataManagerInterface.h" 
#include "C_DataMangerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRPG_API UC_DataMangerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	static bool loadData(IC_DataManagerInterface* pInterface);

private:
	static bool loadData(IC_DataManagerInterface* pInterface, E_DataType DataType);
public:
	static bool saveStringData(IC_DataManagerInterface* pInterface);
	static bool saveStringArrayData(IC_DataManagerInterface* pInterface);
	static bool saveBinaryData(IC_DataManagerInterface* pInterface);

private:
	static bool loadStringData(const FString& FilePath, FString& rResult);
	static bool loadStringArrayData(const FString& FilePath, TArray<FString>& rResult);
	static bool loadBinaryData(const FString& FilePath, TArray<uint8>& rResult);

public:
	template<typename T>
	static bool saveBinaryFile(TArray<uint8>& BinaryArray, T* Content)
	{
		FMemoryWriter Write(BinaryArray, true);
		Write << Content;
		return true;
	}

	template<typename T>
	static bool readBinaryFile(const TArray<uint8>& BinaryArray, T* Content)
	{
		if (!Content)
			return false;
		FMemoryReader Read(BinaryArray, true);
		Read << Content;
		return true;
	}
};
