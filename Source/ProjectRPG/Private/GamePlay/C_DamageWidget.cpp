#include "GamePlay/C_DamageWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UC_DamageWidget::drawDamage(float Damage, FColor Color)
{
	int nSize = m_arDamageText.Num();
	UTextBlock* pText{};
	for (int i = 0; i < nSize && m_arDamageText[i] && m_arDamageText[i]->GetVisibility() == ESlateVisibility::Hidden; i++)
	{
		pText = m_arDamageText[i];
	}

	if (!pText)
		pText = createDataTextBlock();
	drawDamage(pText, Damage, Color);
}

void UC_DamageWidget::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();
	m_arDamageText.Reset(0);
	m_arDamageText.Reserve(10);
	m_arDamageTime.Reserve(10);
	TArray<UWidget*> ChildWidget;
	WidgetTree->GetAllWidgets(ChildWidget);
	for (UWidget*& pWidget : ChildWidget)
	{
		if (UTextBlock* pTextBlock = Cast<UTextBlock>(pWidget))
		{
			m_arDamageText.Add(pTextBlock);
			InitSetting(pTextBlock);
			m_arDamageTime.Add(0);
		}
	}
	if (!m_pCanvasPanel)
	{
		m_pCanvasPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
		WidgetTree->RootWidget = m_pCanvasPanel;
		//rootw
	}
}

void UC_DamageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);
	//m_arDamageText
	int nSize = m_arDamageText.Num();
	UTextBlock* pText{};
	for (int i = 0; i < nSize; i++)
	{
		if (m_arDamageTime[i] >= m_fUpDistance && m_arDamageText[i])
		{
			endText(m_arDamageText[i]);
			m_arDamageTime[i] = 0;
		}
		if (m_arDamageText[i] && m_arDamageText[i]->GetVisibility() != ESlateVisibility::Hidden)
		{
			float Distance = InDeltaTime * m_fUpSpeed;
			if (m_arDamageTime[i] + Distance > m_fUpDistance)
			{
				Distance = m_fUpDistance - m_arDamageTime[i];
			}
			m_arDamageTime[i] += Distance;
			UCanvasPanelSlot* CanvasSlot = Cast< UCanvasPanelSlot>(m_arDamageText[i]->Slot);
			if (CanvasSlot)
			{
				CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, -Distance));
			}
		}
	}
}

UTextBlock* UC_DamageWidget::createDataTextBlock()
{
	if (!m_pCanvasPanel)
	{
		UE_LOG(LogTemp, Error, TEXT("Widget's CanvasPanel is nullptr"));
		return nullptr;
	}
	UTextBlock* pTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	InitSetting(pTextBlock);
	m_arDamageText.Add(pTextBlock);
	m_arDamageTime.Add(0);
	return pTextBlock;
}

void UC_DamageWidget::drawDamage(UTextBlock* TextBlock, float Damage, FColor Color)
{
	if (!TextBlock)
		return;
	m_pCanvasPanel->AddChild(TextBlock);
	UCanvasPanelSlot* CanvasSlot = Cast< UCanvasPanelSlot>(TextBlock->Slot);
	if (CanvasSlot)
	{
		CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
		CanvasSlot->SetPosition(FVector2D(FMath::FRandRange(-10.f,10.f), 0));
	}
	TextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)Damage)));
	TextBlock->SetColorAndOpacity(FSlateColor(Color));
	TextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UC_DamageWidget::InitSetting(UTextBlock* TextBlock)
{
	if (!TextBlock)
		return;
	FSlateFontInfo Info = TextBlock->GetFont();
	Info.OutlineSettings.OutlineSize = 2;
	TextBlock->SetFont(Info);
	TextBlock->SetJustification(ETextJustify::Center);
	TextBlock->SetVisibility(ESlateVisibility::Hidden);
}

void UC_DamageWidget::endText(UTextBlock* TextBlock)
{
	if (!TextBlock)
		return; 
	TextBlock->SetVisibility(ESlateVisibility::Hidden);
	TextBlock->RemoveFromParent();
}
