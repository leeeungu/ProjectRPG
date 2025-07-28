#include "C_MainWidget.h"
#include <Components/CanvasPanelSlot.h>

UC_MainWidget::UC_MainWidget(const FObjectInitializer& ObjectInitializer)
	:UUserWidget{ ObjectInitializer }
{
}


bool UC_MainWidget::addWidgetToMain(E_WindwoType eType, UC_GameWindowWidget* pWidget)
{
	uint8 index = (uint8)eType;
	if (!m_arCanvasPanel.IsValidIndex(index) || !pWidget)
		return false;

	UCanvasPanel* pCanvas = m_arCanvasPanel[index];
	if (pCanvas->HasChild(pWidget))
		return false;
	UCanvasPanelSlot* pSlot = pCanvas->AddChildToCanvas(pWidget);
	if (pSlot)
	{
		pSlot->SetAnchors(FAnchors(0, 0, 1, 1));
	}
	pWidget->addWidgetToMain();
	pWidget->setHasParent(pWidget->GetParent()  == pCanvas);

	return true;
}

bool UC_MainWidget::removeWidgetFromMain(UC_GameWindowWidget* pWidget)
{
	if (!pWidget)
		return false;
	pWidget->removeWidgetFromMain();
	pWidget->setHasParent(pWidget->GetParent()  != nullptr);
	return true;
}

void UC_MainWidget::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();
	m_arCanvasPanel.Init(nullptr, (uint8)E_WindwoType::E_Max);
	m_arCanvasPanel[(uint8)E_WindwoType::E_Inventory] = m_pInventoryCanvas;
	m_arCanvasPanel[(uint8)E_WindwoType::E_Alert] = m_pAlertCanvas;
	m_arCanvasPanel[(uint8)E_WindwoType::E_QuickSlot] = m_pQuickSlot;
}



