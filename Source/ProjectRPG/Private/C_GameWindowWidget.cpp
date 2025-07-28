#include "C_GameWindowWidget.h"

UC_GameWindowWidget::UC_GameWindowWidget(const FObjectInitializer& ObjectInitializer) : 
	UUserWidget{ ObjectInitializer }
{
}

void UC_GameWindowWidget::setHasParent(bool bVal)
{
	m_bHasParent = bVal;
}

void UC_GameWindowWidget::removeWidgetFromMain_Implementation()
{
}

void UC_GameWindowWidget::addWidgetToMain_Implementation()
{
}
