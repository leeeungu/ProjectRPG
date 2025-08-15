#include "Quest/C_NPCQuestComponent.h"
#include "QuestAsset.h"

UC_NPCQuestComponent::UC_NPCQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UC_NPCQuestComponent::setCurrentQuestAsset(UQuestAsset* pAsset)
{
	if (m_CurrentQuestAsset)
	{
		m_CurrentQuestAsset->OnQuestSucceed.RemoveDynamic(this, &UC_NPCQuestComponent::onQuestSucceed);
		m_CurrentQuestAsset->OnQuestFail.RemoveDynamic(this, &UC_NPCQuestComponent::onQuestFail);
	}
	m_bQuestAccept = false;
	m_CurrentQuestAsset = pAsset;
	if (!m_CurrentQuestAsset)
		return;
	m_CurrentQuestAsset->OnQuestSucceed.AddDynamic(this, &UC_NPCQuestComponent::onQuestSucceed);
	m_CurrentQuestAsset->OnQuestFail.AddDynamic(this, &UC_NPCQuestComponent::onQuestFail);
}

void UC_NPCQuestComponent::setQuestStart()
{
	m_bQuestAccept = true;	
}

void UC_NPCQuestComponent::BeginPlay()
{
	Super::BeginPlay();
	setCurrentQuestAsset(m_pCurrentQuestAsset1);
}


void UC_NPCQuestComponent::onQuestSucceed(UQuestAsset* pAsset)
{
	setCurrentQuestAsset(m_pCurrentQuestAsset2);
}

void UC_NPCQuestComponent::onQuestFail(UQuestAsset* pAsset)
{
	m_bQuestAccept = false;
}
