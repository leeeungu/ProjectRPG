#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_GameWindowWidget.generated.h"

UENUM(BlueprintType)
enum class E_WindwoType :  uint8
{
	E_NONE			UMETA(Hidden),
	E_Inventory 	UMETA(DisplayName = "Inventory"),
	E_Alert			UMETA(DisplayName = "Alert"),
	E_QuickSlot		UMETA(DisplayName = "QuickSlot"),
	E_SkillBar		UMETA(DisplayName = "SkillBar"),
	E_SkillWindow	UMETA(DisplayName = "SkillWindow"),
	E_Max
};

UCLASS()
class PROJECTRPG_API UC_GameWindowWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	bool m_bHasParent;
public:
	UC_GameWindowWidget(const FObjectInitializer& ObjectInitializer);
	
	void setHasParent(bool bVal);

	UFUNCTION(BlueprintPure)
	bool getHasParent() const { return m_bHasParent; }
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void removeWidgetFromMain();

	virtual void removeWidgetFromMain_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void addWidgetToMain();

	virtual void addWidgetToMain_Implementation();
};
