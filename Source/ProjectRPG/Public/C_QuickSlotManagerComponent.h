#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_QuickSlotManagerComponent.generated.h"

class UC_InventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuickSlotNone);

UENUM(BlueprintType)
enum class E_QuickSlotType : uint8
{
	E_None UMETA(DisplayName = "None", Hidden),
	E_QuickSlot1 UMETA(DisplayName = "Quick Slot 1"),
	E_QuickSlot2 UMETA(DisplayName = "Quick Slot 2"),
	E_QuickSlot3 UMETA(DisplayName = "Quick Slot 3"),
	E_QuickSlot4 UMETA(DisplayName = "Quick Slot 4"),
	E_QuickSlot5 UMETA(DisplayName = "Quick Slot 5"),
	E_QuickSlot_MAX UMETA(Hidden, DisplayName = "Max Quick Slot")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTRPG_API UC_QuickSlotManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnQuickSlotNone m_onQuickSlotNoneDelegate;

private:
	int m_arrQuickSlotItem[(uint8)E_QuickSlotType::E_QuickSlot_MAX];
	UC_InventoryComponent* m_pInventoryComponent;
public:
	UC_QuickSlotManagerComponent();

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	bool useQuickSlot(E_QuickSlotType QuickSlotType, int nCount = 1);

	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void setQuickSlotItem(E_QuickSlotType QuickSlotType, int ItemID);

protected:
	virtual void BeginPlay() override;


};
