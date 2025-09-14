#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "C_DataManagerInterface.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class E_DataType : uint8
{
	E_NONE = 0,
	E_String = 1,
	E_StringArray = 1 << 1,
	E_Binary = 1 << 2,
};

UINTERFACE(MinimalAPI)
class UC_DataManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTRPG_API IC_DataManagerInterface
{
	GENERATED_BODY()
public:
	virtual E_DataType getDataType() = 0;
	virtual FString getFilePath(E_DataType eType) = 0;

	virtual void loadStringData(FString& Data) {};
	virtual void loadStringArrayData(TArray<FString>& arData) {};
	virtual void loadBinaryData(TArray<uint8>& arData) {};

	virtual FString getStringData() { return FString{}; }
	virtual TArray<FString> getStringArrayData() { return  TArray<FString>{};}
	virtual TArray<uint8> getBinaryData() { return TArray<uint8>{}; }
};
