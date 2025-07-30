#include "C_MainWidget.h"
#include <Components/CanvasPanelSlot.h>

UC_MainWidget::UC_MainWidget(const FObjectInitializer& ObjectInitializer)
	:UUserWidget{ ObjectInitializer }
{
	m_arrGameWidget.Init(nullptr, (uint8)E_WindwoType::E_Max);
}


bool UC_MainWidget::addWidgetToMain(E_WindwoType eType)
{
	uint8 index = (uint8)eType;
	if (!m_arrGameWidget.IsValidIndex(index))
		return false;
	UC_GameWindowWidget* pWidget = m_arrGameWidget[index];
	if (!pWidget || pWidget->getIsOpened())
		return false;
	pWidget->setIsOpened(true);
	return true;
}

bool UC_MainWidget::removeWidgetFromMain(E_WindwoType eType)
{
	uint8 index = (uint8)eType;
	if (!m_arrGameWidget.IsValidIndex(index))
		return false;
	UC_GameWindowWidget* pWidget = m_arrGameWidget[index];
	if (!pWidget || !pWidget->getIsOpened())
		return false;
	pWidget->setIsOpened(false);
	return true;
}

bool UC_MainWidget::isWidgetOpened(E_WindwoType eType) const
{
	uint8 index = (uint8)eType;
	if (!m_arrGameWidget.IsValidIndex(index))
		return false;
	UC_GameWindowWidget* pWidget = m_arrGameWidget[index];
	if (!pWidget)
		return false;
	return pWidget->getIsOpened();
}

void UC_MainWidget::registerWidget(UC_GameWindowWidget* pWidget)
{
	E_WindwoType eType = pWidget->getWindowType();
	uint8 index = (uint8)eType;	
	if (!m_arrGameWidget.IsValidIndex(index))
		return;
	if (m_arrGameWidget[index])
		return;
	m_arrGameWidget[index] = pWidget;
}

void UC_MainWidget::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();
}	



