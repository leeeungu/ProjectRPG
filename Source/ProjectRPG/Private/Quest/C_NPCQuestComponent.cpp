#include "Quest/C_NPCQuestComponent.h"
#include "QuestAsset.h"
#include "QuestNodeInfo.h"
#include "QuestRuntimeGraph.h"
#include "QuestEndNodeInfo.h"

UC_NPCQuestComponent::UC_NPCQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

// Runtime노드에서 현재 NCP 대사를 가져오는 함수
FText UC_NPCQuestComponent::getQuestText() const
{
	if (!m_CurrentQuestAsset)
		return  FText::FromString(TEXT("퀘스트가 없습니다."));
	if (m_CurrentQuestAsset->GetQuestAccept())
		return FText::FromString(TEXT("퀘스트가 이미 진행 중입니다."));
	if (!m_pRunTimeNode)
		return FText::FromString(TEXT("진행 가능한 퀘스트가 없습니다."));
	if (UQuestNodeInfo* pNode = Cast< UQuestNodeInfo>(m_pRunTimeNode->QuestInfo))
	{
		return pNode->QuestText;
	}
	else
	{
		if (UQuestEndNodeInfo* pInfo = Cast< UQuestEndNodeInfo>(m_pRunTimeNode->QuestInfo))
		{
			if (pInfo->Action == EQuestNodeAction::None)
				return FText{};
		}
	}
	return FText::FromString(TEXT("진행 가능한 퀘스트가 없습니다."));
}

// questAseet으로 부터 Runtime노드를 가져와 NCP 대사를 가져오는 함수
bool UC_NPCQuestComponent::startTalk()
{
	m_nQuestAssetIndex = 0;
	while (!m_CurrentQuestAsset && m_arrQuestAsset.IsValidIndex(m_nQuestAssetIndex))
	{
		m_CurrentQuestAsset = m_arrQuestAsset[m_nQuestAssetIndex];
		m_nQuestAssetIndex++;
		if (m_CurrentQuestAsset && m_CurrentQuestAsset->GetFinishQuest() && !m_CurrentQuestAsset->GetQuestAccept())
			m_CurrentQuestAsset = nullptr;
	}
	if (!m_CurrentQuestAsset)
		return false;
	setNextText(m_CurrentQuestAsset->GetQuestStartNode());
	if (!m_pRunTimeNode)
		return false;
	return true;
}

void UC_NPCQuestComponent::setNextText(UQuestRuntimeNode* pNext)
{
	m_pRunTimeNode = pNext;
	if (m_onQuestChanged.IsBound())
	{
		m_onQuestChanged.Broadcast(m_pRunTimeNode != nullptr);
	}
	if (m_pRunTimeNode)
	{
		if (UQuestEndNodeInfo* pInfo = Cast< UQuestEndNodeInfo>(m_pRunTimeNode->QuestInfo))
		{
			if (m_onQuestEnd.IsBound())
				m_onQuestEnd.Broadcast(m_pRunTimeNode);
		}
	}
}

void UC_NPCQuestComponent::endTalk()
{
	m_CurrentQuestAsset = nullptr;
	m_pRunTimeNode = nullptr;
	m_onQuestChanged.Clear();
	m_onQuestEnd.Clear();
}

bool UC_NPCQuestComponent::getResponeNode(int nIndex, FText& NodeText, UQuestRuntimeNode*& pNode) const
{
	if (!m_pRunTimeNode)
		return false;
	if (m_CurrentQuestAsset->GetQuestAccept())
		return  false;
	if (UQuestNodeInfo* pQuestNode = Cast< UQuestNodeInfo>(m_pRunTimeNode->QuestInfo))
	{
		if (pQuestNode->QuestResponse.IsValidIndex(nIndex))
		{
			NodeText = pQuestNode->QuestResponse[nIndex];
			pNode = m_pRunTimeNode->GetNextNode(nIndex);

			return true;
		}
	}
	return false;
}

bool UC_NPCQuestComponent::getResponeSize(int& nIndex) const
{
	if (!m_pRunTimeNode)
		return false;
	if (m_CurrentQuestAsset->GetQuestAccept())
		return  false;
	if (UQuestNodeInfo* pQuestNode = Cast< UQuestNodeInfo>(m_pRunTimeNode->QuestInfo))
	{
		nIndex = pQuestNode->QuestResponse.Num();
			return true;
	}
	return false;
}

void UC_NPCQuestComponent::BeginPlay()
{
	m_nQuestAssetIndex = 0;
	while (!m_CurrentQuestAsset && m_arrQuestAsset.IsValidIndex(m_nQuestAssetIndex))
	{
		m_CurrentQuestAsset = m_arrQuestAsset[m_nQuestAssetIndex];
		m_nQuestAssetIndex++;
		if (m_CurrentQuestAsset && m_CurrentQuestAsset->GetFinishQuest() && !m_CurrentQuestAsset->GetQuestAccept())
			m_CurrentQuestAsset = nullptr;
	}
	Super::BeginPlay();
}

