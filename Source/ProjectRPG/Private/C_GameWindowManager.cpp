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

void UC_GameWindowManager::setStoreMode(bool bSetStoreMode)
{
	if (bSetStoreMode)
	{
		//runWidgetFunc({ E_WindowType::E_QuickSlot }, &UC_GameWindowManager::removeWidgetFromMain);
		runWidgetFunc({ E_WindowType::E_Inventory, E_WindowType::E_Store }, &UC_GameWindowManager::addWidgetToMain);
	}
	else
	{
		runWidgetFunc({ E_WindowType::E_Inventory, E_WindowType::E_Store }, &UC_GameWindowManager::removeWidgetFromMain);
		//runWidgetFunc({ E_WindowType::E_QuickSlot }, &UC_GameWindowManager::addWidgetToMain);
	}
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

void UC_GameWindowManager::runWidgetFunc(std::initializer_list<E_WindowType> arrWidget, bool(UC_GameWindowManager::* pFunc)(E_WindowType))
{
	/**
	* initializer_list 는 연속된 배열에 저장된다는 것 이 보장이 되서
	* E_WindowType*, int 인 함수 프로토 타임을 std::initializer_list<E_WindowType>으로 변경 했음
	*/
	int nSize = arrWidget.size();
	const E_WindowType* arWidgets = arrWidget.begin();
	for (int i = 0 ; i < nSize && pFunc; i ++)
	{
		(this->*pFunc)(arWidgets[i]);
	}
}
