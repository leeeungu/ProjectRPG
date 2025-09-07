#include "BlueprintFunctionLibrary/C_GameDataUtill.h"
#include "GamePlay/C_DataManagerInterface.h"
#include "GamePlay/C_GameInstance.h"
#include "JsonObjectConverter.h"

UC_DataManager* UC_GameDataUtill::getDataManager(UWorld* pWorld)
{
	if (!pWorld)
		return  nullptr;
	UC_GameInstance* Instance = Cast<UC_GameInstance>(pWorld->GetGameInstance());
	if (!Instance)
		return nullptr;
	return Instance->getDataManager();

	//FJsonObjectConverter::JsonObjectStringToUStruct();

}

void UC_GameDataUtill::saveStringData(IC_DataManagerInterface* pInterface)
{
	if (!pInterface)
		return;
	FFileHelper::SaveStringToFile(pInterface->getStringData(), *pInterface->getFilePath( E_DataType::E_String));
}

void UC_GameDataUtill::saveStringArrayData(IC_DataManagerInterface* pInterface)
{
	if (!pInterface)
		return;
	FFileHelper::SaveStringArrayToFile(pInterface->getStringArrayData(), *pInterface->getFilePath(E_DataType::E_StringArray));
}

void UC_GameDataUtill::saveBinaryData(IC_DataManagerInterface* pInterface)
{
	if (!pInterface)
		return;
	FFileHelper::SaveArrayToFile(pInterface->getBinaryData(), *pInterface->getFilePath(E_DataType::E_Binary));
}
