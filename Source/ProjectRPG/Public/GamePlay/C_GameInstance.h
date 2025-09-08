#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "C_GameInstance.generated.h"


UCLASS()
class PROJECTRPG_API UC_GameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;
};
