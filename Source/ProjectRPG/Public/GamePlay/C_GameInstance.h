#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "C_GameInstance.generated.h"


UCLASS()
class PROJECTRPG_API UC_GameInstance : public UGameInstance
{
	GENERATED_BODY()


protected:
	UPROPERTY()
	TSoftObjectPtr<UWorld> m_pTargetLevel{};
	int m_nCount{};
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable)
	static void loadLevelStatic(UObject* pWorldContext);
	UFUNCTION(BlueprintCallable)
	static void openLevelStatic(UObject* pWorldContext, TSoftObjectPtr<UWorld> newLevel);
private:
	UFUNCTION()
	void openLoadedLevel();
	UFUNCTION()
	void progressFunction();
};
