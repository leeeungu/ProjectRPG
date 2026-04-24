#include "C_GameWindowManager.h"
#include <GameFramework/PlayerController.h>
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>
#include "C_MainWidget.h"
#include "EnhancedInputComponent.h"

UC_GameWindowManager::UC_GameWindowManager() : UActorComponent{}
{
	PrimaryComponentTick.bCanEverTick = false;
	///Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_Main.WBP_Main'
	//Script/EnhancedInput.InputAction'/Game/RPG_Player/Input/Actions/Window/IA_ExitGame.IA_ExitGame'
	ConstructorHelpers::FObjectFinder<UInputAction> IA_FAction(TEXT("/Game/RPG_Player/Input/Actions/Window/IA_ExitGame.IA_ExitGame"));
	if (IA_FAction.Succeeded())
	{
		m_pExitButton = IA_FAction.Object;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("NOne"));
	}
}

bool UC_GameWindowManager::toggleWidget(E_WindowType eType)
{
	if (isWidgetOpened(eType))
	{
		return removeWidgetFromMain(eType);
	}
	return addWidgetToMain(eType);
}

void UC_GameWindowManager::toggleWindow(E_WindowType eType)
{
	toggleWidget(eType);
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
	if (!m_pMainWidget)
		return nullptr;
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

void UC_GameWindowManager::setNPCMode(bool bMode)
{
	int i = 1;
	for (i; i < (uint8)E_WindowType::E_Max; i++)
	{
		removeWidgetFromMain((E_WindowType)i);
	}
	if (bMode)
	{
		runWidgetFunc({ E_WindowType::E_NPCWidget }, &UC_GameWindowManager::addWidgetToMain);
	}
	else
	{
		runWidgetFunc({ E_WindowType::E_QuickSlot }, &UC_GameWindowManager::addWidgetToMain);
	}
}

void UC_GameWindowManager::BeginPlay()
{
	UActorComponent::BeginPlay();
	m_pPlayer = Cast< APlayerController>(GetOwner());
	if (!m_pPlayer)
		return;

	if(!m_cMainWidget)
	{
		m_cMainWidget = LoadClass<UC_MainWidget>(nullptr, TEXT("/Game/UI/WBP_Main.WBP_Main_C"), nullptr, LOAD_None, nullptr);
	}
	m_pMainWidget = CreateWidget<UC_MainWidget>(m_pPlayer, m_cMainWidget, TEXT("MainWidget"));
	if (!m_pMainWidget)
		return;
	m_pMainWidget->AddToViewport();
	UE_LOG(LogTemp, Log, TEXT("Toggle"));
	if (m_pPlayer && m_pExitButton)
	{
		UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(m_pPlayer->InputComponent);
		if (EnhancedInput)
		{
			EnhancedInput->BindAction(m_pExitButton, ETriggerEvent::Completed, this, &UC_GameWindowManager::toggleWindow, E_WindowType::E_ExitWidget);
		}
	}
}

void UC_GameWindowManager::OnRegister()
{
	UActorComponent::OnRegister();
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
