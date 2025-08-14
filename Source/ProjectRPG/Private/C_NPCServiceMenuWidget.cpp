#include "C_NPCServiceMenuWidget.h"
#include <Components/Button.h>
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

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

void UC_NPCServiceMenuWidget::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();
	if (m_pServiceButton)
		m_pServiceButton->OnClicked.AddDynamic(this, &UC_NPCServiceMenuWidget::buttonClick);
}

void UC_NPCServiceMenuWidget::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();
	if (m_pServiceText)
		m_pServiceText->SetText(m_streServiceText);

	if (m_pServiceImage)
	{
		FSlateBrush brush{};
		brush.DrawAs = ESlateBrushDrawType::Image;
		if (!m_pServiceTexture)
		{
			brush.DrawAs = ESlateBrushDrawType::NoDrawType;
		}
		m_pServiceImage->SetBrushFromTexture(m_pServiceTexture);
	}
}

void UC_NPCServiceMenuWidget::buttonClick()
{
	if (m_pServiceButton && m_pServiceButton->IsHovered() && !m_bClicked)
	{
		m_bClicked = true;
		onServiceButton();
		if (m_onServiceClicked.IsBound())
			m_onServiceClicked.Broadcast(this);
	}
}
