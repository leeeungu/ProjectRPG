#include "Quest/C_QuestAlertWidgetComponent.h"
#include "Quest/C_NPCAlert.h"
#include "C_NPCBase.h" 
#include "Quest/C_NPCQuestComponent.h"
#include "QuestAsset.h"

UC_QuestAlertWidgetComponent::UC_QuestAlertWidgetComponent(const FObjectInitializer& ObjectInitializer) :
	UWidgetComponent{ ObjectInitializer }
{
	WidgetClass = UC_NPCAlert::StaticClass();
	Space = EWidgetSpace::Screen;
}

void UC_QuestAlertWidgetComponent::BeginPlay()
{
	UWidgetComponent::BeginPlay();
	AC_NPCBase* Character = Cast< AC_NPCBase>(GetOwner());
	if (Character)
	{
		UC_NPCQuestComponent* Component = Character->GetComponentByClass<UC_NPCQuestComponent>();
		if (Component)
		{
			m_pQuest = Component->getCurrentQuestAsset();
			if (m_pQuest && GetWidget())
			{
			}
		}
	}
	if (GetWidget())
		GetWidget()->SetVisibility(ESlateVisibility::Hidden);

}

void UC_QuestAlertWidgetComponent::OnRegister()
{
	UWidgetComponent::OnRegister();
}

void UC_QuestAlertWidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UWidgetComponent::EndPlay(EndPlayReason);
	if (m_pQuest)
	{
		m_pQuest->OnQuestSucceed.RemoveDynamic(this, &UC_QuestAlertWidgetComponent::changeQuest);
		m_pQuest = nullptr;
	}
}

void UC_QuestAlertWidgetComponent::changeQuest(UQuestAsset* Quest)
{
	if (true)
	return;
		UE_LOG(LogTemp, Error, TEXT("Widget's CanvasPanel is nullptr"));
	if (m_pQuest != Quest)
		return;
	m_pQuest = nullptr;
	Quest->OnQuestSucceed.RemoveDynamic(this, &UC_QuestAlertWidgetComponent::changeQuest);
	AC_NPCBase* Character = Cast< AC_NPCBase>(GetOwner());
	if (Character)
	{
		UC_NPCQuestComponent* Component = Character->GetComponentByClass<UC_NPCQuestComponent>();
		if (Component)
		{
			m_pQuest = Component->getCurrentQuestAsset();
			UE_LOG(LogTemp, Error, TEXT("Widget's CanvasPanel is nullptr"));
			if (m_pQuest && GetWidget())
			{
				m_pQuest->OnQuestSucceed.AddDynamic(this,  &UC_QuestAlertWidgetComponent::changeQuest);
				GetWidget()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
	}

	if (!m_pQuest && GetWidget())
		GetWidget()->SetVisibility(ESlateVisibility::Hidden);
}
