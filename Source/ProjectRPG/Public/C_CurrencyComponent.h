#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <C_ItemDataSubsystem.h>
#include "C_CurrencyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPushCurrency, int, nItemID, int, nItemCount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_CurrencyComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "UC_CurrencyComponent")
	FOnPushCurrency m_onPushItem;

private:
	UPROPERTY(VisibleAnywhere, Category = "UC_CurrencyComponent")
	int m_nInventorySize;
	UPROPERTY(VisibleAnywhere, Category = "UC_CurrencyComponent")
	TMap<int, int> m_mapInventory;

	UC_ItemDataSubsystem* m_pItemDataSubsystem;
public:	
	UC_CurrencyComponent();

	/**
	* CurrencyID에 따른 CurrencyAmount를 반환합니다.
	* 해당 CurrencyID가 존재하지 않으면 false를 반환합니다.
	*/
	UFUNCTION(BlueprintPure, Category = "UC_CurrencyComponent")
	bool getCurrency(int nCurrencyID, int& nCurrencyAmount) const;

	UFUNCTION(BlueprintPure, Category = "UC_CurrencyComponent")
	bool hasCurrencyAmount(int nCurrencyID, int nCurrencyAmount) const;

	/**
	* CurrencyID에 CurrencyAmount를 추가합니다.
	* 추가 후, m_onPushItem 이벤트를 발생시킵니다.
	* m_mapInventory에 CurrencyID가 존재하지 않으면 새로 추가합니다.
	* 추가 성공하면 true를 반환합니다.
	*/
	UFUNCTION(BlueprintCallable, Category = "UC_CurrencyComponent")
	bool pushCurrency(int nCurrencyID, int nCurrencyAmount);

	/**
	* CurrencyID에 CurrencyAmount를	제거합니다.
	* 제거에 성공하면 true를 반환합니다.
	*/
	UFUNCTION(BlueprintCallable, Category = "UC_CurrencyComponent")
	bool popCurrency(int nCurrencyID, int nCurrencyAmount);

protected:
	virtual void BeginPlay() override;

};
