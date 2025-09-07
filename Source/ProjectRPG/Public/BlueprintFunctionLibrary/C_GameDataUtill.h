#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "C_GameDataUtill.generated.h"

class IC_DataManagerInterface;
class UC_DataManager;

UCLASS()
class PROJECTRPG_API UC_GameDataUtill : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	static UC_DataManager* getDataManager(UWorld* pWorld);

	static void saveStringData(IC_DataManagerInterface* pInterface);
	static void saveStringArrayData(IC_DataManagerInterface* pInterface);
	static void saveBinaryData(IC_DataManagerInterface* pInterface);
	

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
