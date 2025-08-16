#include "ActorComponent/QuestManagerComponent.h"
#include "QuestEndNodeInfo.h"
#include "QuestAsset.h"
#include "GameFramework/Controller.h"
#include "Object/QuestObject.h"

DEFINE_LOG_CATEGORY_STATIC(QuestManagerComponent, Log, All);

UQuestManagerComponent::UQuestManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestManagerComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
	if( !Cast<AController>(GetOwner()))
	{
		UE_LOG(QuestManagerComponent, Error, TEXT("UQuestManagerComponent : Owner is Not Controller"));
	}
}

void UQuestManagerComponent::QuestEnd(UQuestAsset* pQuest)
{
	AQuestObject** ppObject = mapQuestObject.Find(pQuest);
	if (!ppObject || !*ppObject)
		return;
	if (OnQuestClear.IsBound())
	{
		OnQuestClear.Broadcast(pQuest);
	}
	(*ppObject)->Destroy();
	(*ppObject) = nullptr;
}

void UQuestManagerComponent::QuestSucceed(UQuestAsset* pQuest)
{
	AQuestObject** ppObject = mapQuestObject.Find(pQuest);
	if (!ppObject || !*ppObject)
		return;
	if (OnQuestClear.IsBound())
	{
		OnQuestClear.Broadcast(pQuest);
	}
	(*ppObject)->Destroy();
	(*ppObject) = nullptr;
	pQuest->OnQuestSucceed.RemoveDynamic(this, & UQuestManagerComponent::QuestSucceed);
}

void UQuestManagerComponent::QuestFail(UQuestAsset* pQuest)
{
	AQuestObject** ppObject = mapQuestObject.Find(pQuest);
	if (!ppObject || !*ppObject)
		return;
	(*ppObject)->Destroy();
	(*ppObject) = nullptr;
	pQuest->OnQuestFail.RemoveDynamic(this, &UQuestManagerComponent::QuestFail);
}

bool UQuestManagerComponent::pushQuest(UQuestAsset* pQuest)
{
	if (!pQuest)
	{
		UE_LOG(QuestManagerComponent, Error, TEXT("UQuestManagerComponent : UQuestAsset is nullptr, Module Error"));
		return false;
	}
	else if (!pQuest->GetQuestObjectClass() )
	{
		UE_LOG(QuestManagerComponent, Error, TEXT("UQuestManagerComponent : UQuestAsset is nullptr"));
		return false;
	}
	if (mapQuestObject.Find(pQuest))
		return false;
	AQuestObject*& ObjectRef = mapQuestObject.Add(pQuest);

	ObjectRef = NewObject< AQuestObject>(this, pQuest->GetQuestObjectClass());
	AController* pController= Cast<AController>(GetOwner());
	if (!pController || !ObjectRef)
		return false;

	ObjectRef->InitQuest(pController->GetPawn(), pQuest);
	pQuest->OnQuestFail.AddDynamic(this, &UQuestManagerComponent::QuestFail);
	pQuest->OnQuestSucceed.AddDynamic(this, &UQuestManagerComponent::QuestSucceed);
	return true;
}