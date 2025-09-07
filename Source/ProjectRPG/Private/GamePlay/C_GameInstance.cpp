#include "GamePlay/C_GameInstance.h"
#include "GamePlay/C_DataManager.h"
#include "Kismet/GameplayStatics.h"

void UC_GameInstance::Init()
{
	UGameInstance::Init();
	//UGameplayStatics::OpenLevel();
	m_pDataManager = NewObject<UC_DataManager>(this, TEXT("DataMager"));
	m_pDataManager->loadDataFiles();
}

void UC_GameInstance::Shutdown()
{
	UGameInstance::Shutdown();
	m_pDataManager = nullptr;
}


//friend FArchive& operator<<(FArchive& Ar, FPlayerData* Data)
//{
//	Ar << Data->ID;
//	Ar << Data->Level;
//	Ar << Data->Hp;
//	Ar << Data->Ap;
//	Ar << Data->MoveSpeed;
//	return Ar;
//}