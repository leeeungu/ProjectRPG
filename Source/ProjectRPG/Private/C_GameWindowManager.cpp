#include "C_GameWindowManager.h"
#include <GameFramework/PlayerController.h>
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>
#include "C_MainWidget.h"

UC_GameWindowManager::UC_GameWindowManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UC_GameWindowManager::toggleWindowWidget(E_WindwoType eType, UC_GameWindowWidget* pWidget)
{
	if (!pWidget)
		return;

	if (pWidget->getHasParent())
	{
		m_pMainWidget->removeWidgetFromMain(pWidget);
	}
	else
	{
		m_pMainWidget->addWidgetToMain(eType, pWidget);
	}
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