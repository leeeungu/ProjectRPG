#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_StoreComponent.generated.h"

class UC_CurrencyComponent;
class UC_InventoryComponent;
class APlayerController;

USTRUCT(BlueprintType)
struct FS_ItemStoreData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FS_ItemStoreData")
	int nCostItemID{};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FS_ItemStoreData")
	int nCostItemCount{};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FS_ItemStoreData")
	int nBuyItemID{};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FS_ItemStoreData")
	int nBuyItemCount{};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FS_ItemStoreData")
	bool bBuyOnce{};
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "FS_ItemStoreData")
	bool bAlreadyBuy{};
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_StoreComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AC_NPCBase")
	TArray<FS_ItemStoreData> m_arrStoreList{};
	bool m_bIsStroeOpen{};
public:	
	UC_StoreComponent();

	UFUNCTION(BlueprintCallable)
	bool buyItem(UC_CurrencyComponent* pCurrency , UC_InventoryComponent* pInventroy, int nIndex);
	
	UFUNCTION(BlueprintPure)
	bool getStoreData(int nIndex, FS_ItemStoreData& rData) const;
	UFUNCTION(BlueprintCallable)
	bool checkAlreadyHave(int nIndex, APlayerController* Player);
	UFUNCTION(BlueprintCallable)
	void setIsStroeOpen(bool Value);
	UFUNCTION(BlueprintPure)
	bool getIsStroeOpen() const;
protected:
	virtual void BeginPlay() override;
};
