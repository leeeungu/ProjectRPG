#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <queue>
#include "C_GameAlertSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FS_GameAlertSubsystemConfig
{
	GENERATED_BODY()
public:
	FS_GameAlertSubsystemConfig() = default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Alert")
	FText strDefaultAlertMessage = FText::FromString("Default Alert Message"); // Default alert message
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Alert")
	float fDefaultAlertDuration = 1.0f; // Default duration for alert messages
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Alert")
	bool bShowAlertOnScreen = true; // Whether to show the alert on the screen
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlertMessage, FS_GameAlertSubsystemConfig, Message);

UCLASS()
class PROJECTRPG_API UC_GameAlertSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = "Game Alert")
	FOnAlertMessage m_onAlertScrrenMessage;
	UPROPERTY(BlueprintAssignable, Category = "Game Alert")
	FOnAlertMessage m_onAlertDebugMessage;
	UPROPERTY(BlueprintAssignable, Category = "Game Alert")
	FOnAlertMessage m_onAlertMessage;
private:
	std::queue<FS_GameAlertSubsystemConfig> m_queMessage;
	FTimerHandle m_sTimerHandle;
	int m_nQueSize = 5;
	static UC_GameAlertSubsystem* m_pSingletonInstance;	
public:
	UC_GameAlertSubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


	UFUNCTION(BlueprintCallable, Category = "Game Alert")
	void pushAlertMessage(const FS_GameAlertSubsystemConfig& sMesssage);

	static void pushAlertMessage_Cpp(const FS_GameAlertSubsystemConfig& sMesssage);
private:
	void broadcastAlertMessage();
};
