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
	while (!m_CurrentQuestAsset && m_arrQuestAsset.IsValidIndex(m_nQuestAssetIndex))
	{
		m_CurrentQuestAsset = m_arrQuestAsset[m_nQuestAssetIndex];
		m_nQuestAssetIndex++;
		if (m_CurrentQuestAsset && m_CurrentQuestAsset->GetFinishQuest())
			m_CurrentQuestAsset = nullptr;
	}
	setCurrentQuestAsset(m_CurrentQuestAsset);
}


void UC_NPCQuestComponent::onQuestSucceed(UQuestAsset* pAsset)
{
	m_CurrentQuestAsset = nullptr;
	while (!m_CurrentQuestAsset && m_arrQuestAsset.IsValidIndex(m_nQuestAssetIndex))
	{
		m_CurrentQuestAsset = m_arrQuestAsset[m_nQuestAssetIndex];
		m_nQuestAssetIndex++;
		if (m_CurrentQuestAsset && m_CurrentQuestAsset->GetFinishQuest())
			m_CurrentQuestAsset = nullptr;
	}
	setCurrentQuestAsset(m_CurrentQuestAsset);
}

void UC_NPCQuestComponent::onQuestFail(UQuestAsset* pAsset)
{
	m_bQuestAccept = false;
}
