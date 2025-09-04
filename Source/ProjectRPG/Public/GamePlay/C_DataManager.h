#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "C_DataManagerInterface.h" 
#include "C_DataManager.generated.h"


UCLASS()
class PROJECTRPG_API UC_DataManager : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TMap < FString, E_DataType> m_mapDataFile{};
	TMap < FString, FString> m_mapStringData{};
	TMap < FString, TArray<FString>> m_mapStringArrayData{};
	TMap < FString, TArray<uint8>> m_mapBinaryArrayData{};

	TArray <TSoftObjectPtr<IC_DataManagerInterface>> m_arrSaveObjects{};
public:
	void registerDataFile(TSoftObjectPtr<IC_DataManagerInterface> pInterface);

	void loadDataFiles();
	void saveDataFiles();

	bool loadData(IC_DataManagerInterface* pInterface);
private:
	bool loadData(IC_DataManagerInterface* pInterface, E_DataType DataType);
	void loadStringData(const FString& FilePath);
	void loadStringArrayData(const FString& FilePath);
	void loadBinaryData(const FString& FilePath);
};
