#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_StoreComponent.generated.h"

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
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_StoreComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AC_NPCBase")
	TArray<FS_ItemStoreData> m_arrStoreList{};
public:	
	UC_StoreComponent();

protected:
	virtual void BeginPlay() override;
};
