#include "Quest/C_QuestManagerComponent.h"
#include "QuestEndNodeInfo.h"
#include "QuestAsset.h"
#include "GameFramework/Controller.h"
#include "Object/QuestObject.h"

DEFINE_LOG_CATEGORY_STATIC(C_QuestManagerComponent, Log, All);

UC_QuestManagerComponent::UC_QuestManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UC_QuestManagerComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
	if (!Cast<AController>(GetOwner()))
	{
		UE_LOG(C_QuestManagerComponent, Error, TEXT("UC_QuestManagerComponent : Owner is Not Controller"));
	}
}

void UC_QuestManagerComponent::QuestEnd(UQuestAsset* pQuest)
{
	AQuestObject** ppObject = m_mapQuestObject.Find(pQuest);
	m_mapQuestObject.Remove(pQuest);
	if (OnQuestClear.IsBound())
	{
		OnQuestClear.Broadcast(pQuest);
	}
	if (OnQuestPop.IsBound())
	{
		OnQuestPop.Broadcast(pQuest);
	}
	if (ppObject && *ppObject)
	{
		(*ppObject)->Destroy();
		(*ppObject) = nullptr;
	}
}

void UC_QuestManagerComponent::QuestSucceed(UQuestAsset* pQuest)
{
	AQuestObject** ppObject = m_mapQuestObject.Find(pQuest);
	m_mapQuestObject.Remove(pQuest);
	pQuest->SetFinishQuest(true);
	if (OnQuestClear.IsBound())
	{
		OnQuestClear.Broadcast(pQuest);
	}
	if (OnQuestPop.IsBound())
	{
		OnQuestPop.Broadcast(pQuest);
	}
	pQuest->OnQuestSucceed.RemoveDynamic(this, &UC_QuestManagerComponent::QuestSucceed);
	pQuest->OnQuestFail.RemoveDynamic(this, &UC_QuestManagerComponent::QuestFail);
	if (ppObject && *ppObject)
	{
		(*ppObject)->Destroy();
		(*ppObject) = nullptr;
	}
}

void UC_QuestManagerComponent::QuestFail(UQuestAsset* pQuest)
{
	AQuestObject** ppObject = m_mapQuestObject.Find(pQuest);
	m_mapQuestObject.Remove(pQuest);
	pQuest->OnQuestFail.RemoveDynamic(this, &UC_QuestManagerComponent::QuestFail);
	pQuest->OnQuestSucceed.RemoveDynamic(this, &UC_QuestManagerComponent::QuestSucceed);
	if (OnQuestPop.IsBound())
	{
		OnQuestPop.Broadcast(pQuest);
	}
	if (ppObject && *ppObject)
	{
		(*ppObject)->Destroy();
		(*ppObject) = nullptr;
	}
}

bool UC_QuestManagerComponent::pushQuest(UQuestAsset* pQuest)
{
	if (!pQuest)
	{
		UE_LOG(C_QuestManagerComponent, Error, TEXT("UC_QuestManagerComponent : UQuestAsset is nullptr, Module Error"));
		return false;
	}
	else if (!pQuest->GetQuestObjectClass())
	{
		UE_LOG(C_QuestManagerComponent, Error, TEXT("UC_QuestManagerComponent : UQuestAsset is nullptr"));
		return false;
	}
	if (m_mapQuestObject.Find(pQuest))
		return false;
	AQuestObject*& ObjectRef = m_mapQuestObject.Add(pQuest);

	pQuest->OnQuestFail.AddDynamic(this, &UC_QuestManagerComponent::QuestFail);
	pQuest->OnQuestSucceed.AddDynamic(this, &UC_QuestManagerComponent::QuestSucceed);
	if (OnQuestPush.IsBound())
	{
		OnQuestPush.Broadcast(pQuest);
	}
	return true;
}

TArray<UQuestAsset*> UC_QuestManagerComponent::getQuestAsset() const
{
	TArray<UQuestAsset*> arr{};
	m_mapQuestObject.GetKeys(arr);
	return arr;
}

bool UC_QuestManagerComponent::getHasQuest(UQuestAsset* pQuest) const
{
	AQuestObject*const* ppObject = m_mapQuestObject.Find(pQuest);
	return ppObject != nullptr;
}

bool UC_QuestManagerComponent::clearQuest(UQuestAsset* pQuest, bool bSucceed)
{
	AQuestObject* const* ppObject = m_mapQuestObject.Find(pQuest);
	if (ppObject == nullptr)
		return false;
	if (bSucceed)
		QuestSucceed(pQuest);
	else
		QuestFail(pQuest);
	return true;
}

E_DataType UC_QuestManagerComponent::getDataType()
{
	return E_DataType::E_Binary;
}

FString UC_QuestManagerComponent::getFilePath(E_DataType eType)
{
	return FPaths::ProjectSavedDir() + TEXT("QuestSave");
}

void UC_QuestManagerComponent::loadBinaryData(TArray<uint8>& arData)
{
}

TArray<uint8> UC_QuestManagerComponent::getBinaryData()
{
	return TArray<uint8>();
}
