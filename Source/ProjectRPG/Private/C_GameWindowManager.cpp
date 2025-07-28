#include "C_GameWindowManager.h"
#include <GameFramework/PlayerController.h>
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>
#include "C_MainWidget.h"

UC_GameWindowManager::UC_GameWindowManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UC_GameWindowManager::toggleWidget(E_WindwoType eType)
{
	if (isWidgetOpened(eType))
	{
		return removeWidgetFromMain(eType);
	}
	return addWidgetToMain(eType);
}

bool UC_GameWindowManager::removeWidgetFromMain(E_WindwoType eType)
{
	return m_pMainWidget->removeWidgetFromMain(eType);
}

bool UC_GameWindowManager::addWidgetToMain(E_WindwoType eType)
{
	return m_pMainWidget->addWidgetToMain(eType);
}

bool UC_GameWindowManager::isWidgetOpened(E_WindwoType eType) const
{
	return m_pMainWidget->isWidgetOpened(eType);
}

void UC_GameWindowManager::BeginPlay()
{
	UActorComponent::BeginPlay();
	m_pPlayer = Cast< APlayerController>(GetOwner());
	if (!m_pPlayer)
		return;

	m_arWidget.Init(nullptr, (uint8)E_WindwoType::E_Max);
	m_pMainWidget = CreateWidget<UC_MainWidget>(m_pPlayer, m_cMainWidget,TEXT("MainWidget"));
	if (!m_pMainWidget)
		return;
	m_pMainWidget->AddToViewport();
}