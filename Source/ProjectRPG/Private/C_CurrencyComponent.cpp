#include "C_CurrencyComponent.h"

UC_CurrencyComponent::UC_CurrencyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UC_CurrencyComponent::getCurrency(int nCurrencyID, int& nCurrencyAmount) const
{
	if (!m_pItemDataSubsystem || m_pItemDataSubsystem->GetUniqueID() == nCurrencyID)	
		return false;
	const int* pAmount = m_mapInventory.Find(nCurrencyID);
	nCurrencyAmount = 0;
	if (pAmount)
		nCurrencyAmount = *pAmount;
	return pAmount != nullptr;
}

bool UC_CurrencyComponent::hasCurrencyAmount(int nCurrencyID, int nCurrencyAmount) const
{
	if (!m_pItemDataSubsystem || m_pItemDataSubsystem->GetUniqueID() == nCurrencyID)
		return false;
	const int* pAmount = m_mapInventory.Find(nCurrencyID);
	if (pAmount)
		return *pAmount >= nCurrencyAmount;
	return false;
}

bool UC_CurrencyComponent::pushCurrency(int nCurrencyID, int nCurrencyAmount)
{
	if (!m_pItemDataSubsystem || m_pItemDataSubsystem->GetUniqueID() == nCurrencyID)
		return false;
	int& rCurrency = m_mapInventory.FindOrAdd(nCurrencyID);
	if (rCurrency > INT_MAX - nCurrencyAmount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Currency overflow for ID: %d"), nCurrencyID);
		return false; // Prevent overflow
	}
	rCurrency += nCurrencyAmount;
	if (m_onPushItem.IsBound())
		m_onPushItem.Broadcast(nCurrencyID, rCurrency);
	return true;
}

bool UC_CurrencyComponent::popCurrency(int nCurrencyID, int nCurrencyAmount)
{
	if (!m_pItemDataSubsystem || m_pItemDataSubsystem->GetUniqueID() == nCurrencyID)
		return false;
	int* pCurrency = m_mapInventory.Find(nCurrencyID);
	if (!pCurrency || *pCurrency < INT_MIN + nCurrencyAmount)
		return false; // Prevent underflow or none
	*pCurrency -= nCurrencyAmount;
	return true;
}

void UC_CurrencyComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
		m_pItemDataSubsystem = GameInstance->GetSubsystem<UC_ItemDataSubsystem>();
	if (m_pItemDataSubsystem)
		pushCurrency(m_pItemDataSubsystem->getCurrencyGoldItemID(), 0); // Initialize with 0 gold
}

