#include "C_GameWindowWidget.h"
#include "C_MainWidget.h"

UC_GameWindowWidget::UC_GameWindowWidget(const FObjectInitializer& ObjectInitializer) : 
	UUserWidget{ ObjectInitializer }
{
	m_bIsOpened = true;
}

void UC_GameWindowWidget::setIsOpened(bool bVal)
{
	m_bIsOpened = bVal;
	if (m_bIsOpened)
		addWidgetToMain();
	else
		removeWidgetFromMain();
}

void UC_GameWindowWidget::removeWidgetFromMain_Implementation()
{
}

void UC_GameWindowWidget::addWidgetToMain_Implementation()
{
}

void UC_GameWindowWidget::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();
	if (m_eWindowType == E_WindwoType::E_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("UC_GameWindowWidget::NativeOnInitialized() - m_eWindowType is E_NONE"));
	}

	UC_MainWidget* pWidget = GetTypedOuter< UC_MainWidget>();
	setIsOpened(true);
	if (pWidget)
	{
		pWidget->registerWidget(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UC_GameWindowWidget::NativeOnInitialized() - UC_MainWidget is not parent"));
	}
}
