#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Quest/C_GPTInterface.h"
#include "C_VARest.generated.h"

class UVaRestSubsystem;

USTRUCT(BlueprintType)
struct FGPTRequest
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GPTRequest")
	FString Frompt{}; 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GPTRequest")
	FString Text{};
};

UCLASS()
class PROJECTRPG_API UC_VARest : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	UVaRestSubsystem* VaRestSubsystem{};
	// 서버에서 관리가 필요하지만 생략
	FString APIKey = TEXT("YOUR_API_KEY_HERE");
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "VaRest|Utility")
	void SendGPTRequest(FGPTRequest RequestData, TScriptInterface<IC_GPTInterface> Interface, UObject* Target);

	UFUNCTION(BlueprintCallable, Category = "VaRest|Utility")
	FString GetStringFromRequestJSON(class UVaRestRequestJSON* Request);

private:/*
	UFUNCTION()
	void OnRequestComplete(class UVaRestRequestJSON* Request);*/
};
