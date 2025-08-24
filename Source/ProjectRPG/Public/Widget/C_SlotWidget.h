#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_SlotWidget.generated.h"

class UC_SlotFrameBorder;
class UBorder;
class UImage;
class UWidgetAnimation;

UCLASS()
class PROJECTRPG_API UC_SlotWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget), Category = "UC_SlotWidget")
	UBorder* m_pBackGround{};
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget), Category = "UC_SlotWidget")
	UC_SlotFrameBorder* m_pSlotFrame{};
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget), Category = "UC_SlotWidget")
	UImage* m_pSlotImage{};
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget), Category = "UC_SlotWidget")
	UC_SlotFrameBorder* m_pHoverFrame{};
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UC_SlotWidget", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* m_pHoverAnimation{};

public:
	UC_SlotWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void hoverStart();
	void hoverEnd();
};
