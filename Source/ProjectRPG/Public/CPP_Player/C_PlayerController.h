// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "C_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UC_InputQueueComponent;
class UC_CurrencyComponent;
class UC_GameWindowManager;
class UC_QuickSlotManagerComponent;
class UQuestManagerComponent;
class UC_EquipComponent;
class UC_InventoryComponent;
/**
 * 
 */

UENUM(BlueprintType)
enum class EMouseHitType : uint8
{
	None,
	Object,
	Ground
};

UCLASS()
class PROJECTRPG_API AC_PlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	FHitResult CachedMouseHit;
	EMouseHitType CachedHitType = EMouseHitType::None;
	void UpdateMouseHit();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;
	UPROPERTY(VisibleAnywhere)
	UC_InputQueueComponent* InputQueueSystem;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RightClick;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SpaceBar;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* Q_Key;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* R_Key;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* Number1_Key;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* Number2_Key;

	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (DisplayName = "CurrencyComponent"), BlueprintGetter = getCurrencyComponent)
	UC_CurrencyComponent* m_pCurrencyComponent{};
	UPROPERTY(VisibleAnywhere, Category = "WindowWidget", meta = (DisplayName = "WindowManager"), BlueprintGetter = getGameWindowManager)
	UC_GameWindowManager* m_pGameWindowManager{};
	UPROPERTY(VisibleAnywhere, Category = "QuickSlot", meta = (DisplayName = "QuickSlotManagerComponent"), BlueprintGetter = getQuickSlotManagerComponent)
	UC_QuickSlotManagerComponent* m_pQuickSlotManagerComponent{};
	UPROPERTY(VisibleAnywhere, Category = "QuestManager", meta = (DisplayName = "QuestManagerComponent"), BlueprintGetter = getQuestManagerComponent)
	UQuestManagerComponent* m_pQuestManagerComponent{};
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (DisplayName = "EquipComponent"), BlueprintGetter = getEquipComponent)
	UC_EquipComponent* m_pEquipComponent{};
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (DisplayName = "InventoryComponent"), BlueprintGetter = getInventoryComponent)
	UC_InventoryComponent* m_pInventoryComponent{};
public:
	void OnRightClickAction(const FInputActionValue& Value);
	void OnSpaceBarAction(const FInputActionValue& Value);
	void OnQ_Action(const FInputActionValue& Value);
	void OnR_ActionStarted(const FInputActionValue& Value);
	void OnR_ActionOngoing(const FInputActionValue& Value);
	void OnR_ActionCanceld(const FInputActionValue& Value);
	void OnR_ActionCompleted(const FInputActionValue& Value);
	void OnNumber1_Action(const FInputActionValue& Value);
	void OnNumber2_Action(const FInputActionValue& Value);
public:
	AC_PlayerController();
	virtual void OnPossess(APawn* pawn) override;
	// 캐시된 마우스 Hit 결과를 반환
	UFUNCTION(BlueprintCallable, Category = "Mouse")
	bool GetCachedMouseHit(FHitResult& OutHit, EMouseHitType& OutType) const;
	
	UFUNCTION(BlueprintPure, Category = "Inventory")
	UC_CurrencyComponent* getCurrencyComponent() const { return m_pCurrencyComponent; }
	UFUNCTION(BlueprintPure, Category = "WindowWidget")
	UC_GameWindowManager* getGameWindowManager() const { return m_pGameWindowManager; }
	UFUNCTION(BlueprintPure, Category = "QuickSlot")
	UC_QuickSlotManagerComponent* getQuickSlotManagerComponent() const { return m_pQuickSlotManagerComponent; }
	UFUNCTION(BlueprintPure, Category = "QuestManager")
	UQuestManagerComponent* getQuestManagerComponent() const { return m_pQuestManagerComponent; }
	UFUNCTION(BlueprintPure, Category = "Inventory")
	UC_EquipComponent* getEquipComponent() const { return m_pEquipComponent; }
	UFUNCTION(BlueprintPure, Category = "Inventory")
	UC_InventoryComponent* getInventoryComponent() const { return m_pInventoryComponent; }
};
