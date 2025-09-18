#include "GamePlay/C_DamageWidgetComponent.h"
#include "GamePlay/C_DamageWidget.h"
#include "C_BaseCharacter.h" 

UC_DamageWidgetComponent::UC_DamageWidgetComponent(const FObjectInitializer& ObjectInitializer) : 
	UWidgetComponent{ ObjectInitializer }
{
	WidgetClass = UC_DamageWidget::StaticClass();
	Space = EWidgetSpace::Screen;
}

void UC_DamageWidgetComponent::BeginPlay()
{
	UWidgetComponent::BeginPlay();
	AC_BaseCharacter* Character = Cast< AC_BaseCharacter>( GetOwner());
	if (Character)
	{
		Character->m_onChangeHP.AddDynamic(this, &UC_DamageWidgetComponent::changeHp);
	}
	m_pDamageWidget = Cast< UC_DamageWidget>(GetWidget());
}

void UC_DamageWidgetComponent::OnRegister()
{
	UWidgetComponent::OnRegister();
}

void UC_DamageWidgetComponent::changeHp(float fPre, float fCurrent)
{
	if (m_pDamageWidget)
	{
		if (fPre - fCurrent > 0)
			m_pDamageWidget->drawDamage(fPre - fCurrent);
		else if (fCurrent - fPre > 0)
			m_pDamageWidget->drawDamage(fCurrent - fPre , FColor::Green);
	}
}
