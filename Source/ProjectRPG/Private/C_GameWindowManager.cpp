#include "C_GameWindowManager.h"
#include <GameFramework/PlayerController.h>
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>
#include "C_MainWidget.h"

UC_GameWindowManager::UC_GameWindowManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UC_GameWindowManager::toggleWidget(E_WindowType eType)
{
	if (isWidgetOpened(eType))
	{
		return removeWidgetFromMain(eType);
	}
	return addWidgetToMain(eType);
}

bool UC_GameWindowManager::removeWidgetFromMain(E_WindowType eType)
{
	return m_pMainWidget->removeWidgetFromMain(eType);
}

bool UC_GameWindowManager::addWidgetToMain(E_WindowType eType)
{
	return m_pMainWidget->addWidgetToMain(eType);
}

bool UC_GameWindowManager::isWidgetOpened(E_WindowType eType) const
{
	return m_pMainWidget->isWidgetOpened(eType);
}

UC_GameWindowWidget* UC_GameWindowManager::getGameWindowWidget(E_WindowType eType)
{
	return m_pMainWidget->getGameWindowWidget(eType);
}

//bool UC_GameWindowManager::onoffMainWidget(bool bVal)
//{
//	if (!m_pMainWidget || m_bMainWidgetOpened == bVal)
//		return false;
//	m_bMainWidgetOpened = bVal;
//	if (m_bMainWidgetOpened)
//		m_pMainWidget->AddToViewport();
//	else
//		m_pMainWidget->RemoveFromParent();
//	return false;
//}

void UC_GameWindowManager::BeginPlay()
{
	UActorComponent::BeginPlay();
	m_pPlayer = Cast< APlayerController>(GetOwner());
	if (!m_pPlayer)
		return;

	m_pMainWidget = CreateWidget<UC_MainWidget>(m_pPlayer, m_cMainWidget,TEXT("MainWidget"));
	if (!m_pMainWidget)
		return;
	m_pMainWidget->AddToViewport();
}