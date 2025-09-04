#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "C_GameInstance.generated.h"

class UC_DataManager;

UCLASS()
class PROJECTRPG_API UC_GameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UC_DataManager* m_pDataManager{};
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UC_DataManager* getDataManager() { return m_pDataManager; }
};
