#include "C_NPCServiceMenuWidget.h"
#include <Components/Button.h>

UC_NPCServiceMenuWidget::UC_NPCServiceMenuWidget(const FObjectInitializer& ObjectInitializer) :
	UUserWidget{ ObjectInitializer }
{
}

UC_NPCServiceMenuWidget* UC_NPCServiceMenuWidget::unSelectServiceWidget(UC_NPCServiceMenuWidget* pPreWidget)
{
	if (pPreWidget)
	{
		pPreWidget->m_bClicked = false;
		pPreWidget->unSelectService();
	}
	return this;
}

void UC_NPCServiceMenuWidget::serviceButton_Implementation()
{
}

void UC_NPCServiceMenuWidget::unSelectService_Implementation()
{
}

void UC_NPCServiceMenuWidget::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();
	if (m_pServiceButton)
		m_pServiceButton->OnClicked.AddDynamic(this, &UC_NPCServiceMenuWidget::buttonClick);
}

void UC_NPCServiceMenuWidget::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();
}

void UC_NPCServiceMenuWidget::buttonClick()
{
	if (m_pServiceButton && m_pServiceButton->IsHovered() && !m_bClicked)
	{
		m_bClicked = true;
		serviceButton();
		if (m_onServiceClicked.IsBound())
			m_onServiceClicked.Broadcast(this);
	}
}
