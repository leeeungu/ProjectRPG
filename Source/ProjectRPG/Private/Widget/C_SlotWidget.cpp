#include "Widget/C_SlotWidget.h"
#include "Components/Image.h"
#include "Widget/C_SlotFrameBorder.h"
#include "Animation/WidgetAnimation.h"

UC_SlotWidget::UC_SlotWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget{ ObjectInitializer }
{
	
}

void UC_SlotWidget::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();
	if (m_pHoverFrame)
		m_pHoverFrame->SetVisibility(ESlateVisibility::Hidden);
	if (m_pSlotImage)
	{
		FSlateBrush brush{};
		brush.DrawAs = ESlateBrushDrawType::NoDrawType;
		m_pSlotImage->SetBrush(brush);
	}
}

void UC_SlotWidget::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();
	if (m_pSlotImage)
	{
		FSlateBrush brush{};
		brush.DrawAs = ESlateBrushDrawType::NoDrawType;
		m_pSlotImage->SetBrush(brush);
	}
}

void UC_SlotWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();
}

void UC_SlotWidget::NativeDestruct()
{
	UUserWidget::NativeDestruct();
	hoverEnd();
}

void UC_SlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{	
	UUserWidget::NativeOnMouseEnter(InGeometry, InMouseEvent);
	hoverStart();
}

void UC_SlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UUserWidget::NativeOnMouseLeave(InMouseEvent);
	hoverEnd();
}

void UC_SlotWidget::hoverStart()
{
	if (m_pBackGround && m_pBackGround->IsHovered())
	{
		m_pHoverFrame->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (m_pHoverAnimation)
		{
			PlayAnimation(m_pHoverAnimation,0,0);
		}
	}
}

void UC_SlotWidget::hoverEnd()
{
	if (m_pBackGround && !m_pBackGround->IsHovered())
	{
		m_pHoverFrame->SetVisibility(ESlateVisibility::Hidden);
		if (m_pHoverAnimation)
		{
			SetAnimationCurrentTime(m_pHoverAnimation, 1.0f);
			StopAnimation(m_pHoverAnimation);
		}
	}
}
