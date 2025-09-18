#include "GamePlay/C_DamageWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UC_DamageWidget::drawDamage(float Damage, FColor Color)
{
	int nSize = m_arDamageText.Num();
	for (int i = 0; i < nSize; i++)
	{
		if (m_arDamageText[i] && m_arDamageText[i]->GetVisibility() == ESlateVisibility::Hidden)
		{
			drawDamage(m_arDamageText[i], Damage, Color);
		}
	}
}

void UC_DamageWidget::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();
	m_arDamageText.Reset(0);
	TArray<UWidget*> ChildWidget;
	WidgetTree->GetAllWidgets(ChildWidget);
	for (UWidget*& pWidget : ChildWidget)
	{
		if (UTextBlock* pTextBlock = Cast<UTextBlock>(pWidget))
		{
			m_arDamageText.Add(pTextBlock);
			pTextBlock->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UC_DamageWidget::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();
	
}

void UC_DamageWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();
}

void UC_DamageWidget::NativeDestruct()
{
	UUserWidget::NativeDestruct();
}

UTextBlock* UC_DamageWidget::createDataTextBlock()
{
	UTextBlock* pTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	UCanvasPanelSlot* CanvasSlot = Cast< UCanvasPanelSlot>(m_pCanvasPanel->AddChild(pTextBlock));
	if (CanvasSlot)
		CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	pTextBlock->SetJustification(ETextJustify::Center);
	return pTextBlock;
}

void UC_DamageWidget::drawDamage(UTextBlock* TextBlock, float Damage, FColor Color)
{
	if (!TextBlock)
		return;
	TextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)Damage)));
	TextBlock->SetColorAndOpacity(FSlateColor(Color));
	TextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
