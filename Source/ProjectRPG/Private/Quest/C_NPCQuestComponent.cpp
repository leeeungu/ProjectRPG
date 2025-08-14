#include "Quest/C_NPCQuestComponent.h"
#include "QuestAsset.h"

UC_NPCQuestComponent::UC_NPCQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UC_NPCQuestComponent::setCurrentQuestAsset(UQuestAsset* pAsset)
{
	m_pCurrentQuestAsset = pAsset;
	m_bQuestAccept = false;
}

void UC_NPCQuestComponent::setQuestStart()
{
	m_pCurrentQuestAsset = nullptr;
	m_bQuestAccept = true;	
}

void UC_NPCQuestComponent::BeginPlay()
{
	Super::BeginPlay();
}