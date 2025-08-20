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
	mapQuestObject.Remove(pQuest);
	if (OnQuestClear.IsBound())
	{
		OnQuestClear.Broadcast(pQuest);
	}
	if (OnQuestPop.IsBound())
	{
		OnQuestPop.Broadcast(pQuest);
	}
	(*ppObject)->Destroy();
	(*ppObject) = nullptr;
}

void UQuestManagerComponent::QuestSucceed(UQuestAsset* pQuest)
{
	AQuestObject** ppObject = mapQuestObject.Find(pQuest);
	if (!ppObject || !*ppObject)
		return;
	mapQuestObject.Remove(pQuest);
	if (OnQuestClear.IsBound())
	{
		OnQuestClear.Broadcast(pQuest);
	}
	if (OnQuestPop.IsBound())
	{
		OnQuestPop.Broadcast(pQuest);
	}
	pQuest->OnQuestSucceed.RemoveDynamic(this, & UQuestManagerComponent::QuestSucceed);
	pQuest->OnQuestFail.RemoveDynamic(this, &UQuestManagerComponent::QuestFail);
	(*ppObject)->Destroy();
	(*ppObject) = nullptr;
}

void UQuestManagerComponent::QuestFail(UQuestAsset* pQuest)
{
	AQuestObject** ppObject = mapQuestObject.Find(pQuest);
	if (!ppObject || !*ppObject)
		return;
	mapQuestObject.Remove(pQuest);
	pQuest->OnQuestFail.RemoveDynamic(this, &UQuestManagerComponent::QuestFail);
	pQuest->OnQuestSucceed.RemoveDynamic(this, &UQuestManagerComponent::QuestSucceed);
	if (OnQuestPop.IsBound())
	{
		OnQuestPop.Broadcast(pQuest);
	}
	(*ppObject)->Destroy();
	(*ppObject) = nullptr;
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
	if (OnQuestPush.IsBound())
	{
		OnQuestPush.Broadcast(pQuest);
	}
	return true;
}

TArray<UQuestAsset*> UQuestManagerComponent::getQuestAsset() const
{
	TArray<UQuestAsset*> arr{};
	mapQuestObject.GetKeys(arr);
	return arr;
}
