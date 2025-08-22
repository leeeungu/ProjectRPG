#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_GameWindowWidget.generated.h"

UENUM(BlueprintType)
enum class E_WindowType :  uint8
{
	E_NONE			UMETA(Hidden),
	E_Inventory 	UMETA(DisplayName = "Inventory"),
	E_Alert			UMETA(DisplayName = "Alert"),
	E_QuickSlot		UMETA(DisplayName = "QuickSlot"),
	E_Store			UMETA(DisplayName = "Store"),
	E_SkillWindow	UMETA(DisplayName = "SkillWindow"),
	E_QuestWindow	UMETA(DisplayName = "QuestWindow"),
	E_PlayerInfo	UMETA(DisplayName = "PlayerInfo"),
	E_Max UMETA(Hidden),
};

UCLASS()
class PROJECTRPG_API UC_GameWindowWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly ,Category = "UC_GameWindowWidget")
	E_WindowType m_eWindowType;
	UPROPERTY(EditDefaultsOnly, Category = "UC_GameWindowWidget")
	bool m_bIsInitialized;
private:
	bool m_bIsOpened = true;	
public:
	UC_GameWindowWidget(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	void setIsOpened(bool bVal);

	UFUNCTION(BlueprintPure)
	bool getIsOpened() const { return m_bIsOpened; }


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void removeWidgetFromMain();

	virtual void removeWidgetFromMain_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void addWidgetToMain();

	virtual void addWidgetToMain_Implementation();

	E_WindowType getWindowType() const { return m_eWindowType; }


	bool getIsInitialized() const { return m_bIsInitialized; }


	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	virtual void NativeOnInitialized() override;
};
