#include "C_CurrencyComponent.h"

UC_CurrencyComponent::UC_CurrencyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UC_CurrencyComponent::getCurrency(int nCurrencyID, int& nCurrencyAmount) const
{
	int nAmount{};
	nCurrencyAmount = 0;
	if (!getCurrencyCount(nCurrencyID, nAmount))
		return false;
	//UE_LOG(LogTmp, Log, TEXT("%d"), nAmount);
	nCurrencyAmount = nAmount;
	return true;
}

bool UC_CurrencyComponent::hasCurrencyAmount(int nCurrencyID, int nCurrencyAmount) const
{
	int nAmount{};
	if (!getCurrencyCount(nCurrencyID, nAmount))
		return false;
	return nAmount >= nCurrencyAmount;
}

bool UC_CurrencyComponent::pushCurrency(int nCurrencyID, int nCurrencyAmount)
{
	if (!isValueCurrency(nCurrencyID))
		return false;
	int& rCurrency = m_mapInventory.FindOrAdd(nCurrencyID);
	if (rCurrency > INT_MAX - nCurrencyAmount)
	{
		pushAlertMessage(E_AlertType::E_CantPush);
		return false; // Prevent overflow
	}
	rCurrency += nCurrencyAmount;
	if (m_onPushItem.IsBound())
		m_onPushItem.Broadcast(nCurrencyID, rCurrency);
	return true;
}

bool UC_CurrencyComponent::popCurrency(int nCurrencyID, int nCurrencyAmount)
{
	if (!isValueCurrency(nCurrencyID))
		return false;
	int* pCurrency = m_mapInventory.Find(nCurrencyID);
	if (!pCurrency || *pCurrency < INT_MIN + nCurrencyAmount)
	{
		pushAlertMessage(E_AlertType::E_CantPop);
		return false; 
	}
	*pCurrency -= nCurrencyAmount;
	return true;
}

bool UC_CurrencyComponent::useCurrency(int nCurrencyID, int nCurrencyAmount)
{
	int nAmount{};
	if (!getCurrencyCount(nCurrencyID, nAmount))
		return false;
	if (nAmount < nCurrencyAmount || nAmount <= 0)
	{
		pushAlertMessage(E_NotEnough);
		return false;
	}
	return popCurrency(nCurrencyID, nCurrencyAmount);
}

void UC_CurrencyComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		m_pItemDataSubsystem = GameInstance->GetSubsystem<UC_ItemDataSubsystem>();
	}
	if (m_pItemDataSubsystem)
		pushCurrency(m_pItemDataSubsystem->getCurrencyGoldItemID(), 0); // Initialize with 0 gold
}

void UC_CurrencyComponent::pushAlertMessage(E_AlertType eType) const
{
	FS_GameAlertSubsystemConfig config{};
	switch (eType)
	{
	case UC_CurrencyComponent::E_CantPush:
		config.strDefaultAlertMessage = FText::FromString(TEXT("화폐를 더이상 획득할 수 없습니다."));
		break;
	case UC_CurrencyComponent::E_CantPop:
		config.strDefaultAlertMessage = FText::FromString(TEXT("화폐를 더이상 차감할 수 없습니다."));
		break;
	case UC_CurrencyComponent::E_NotCurrency:
		config.strDefaultAlertMessage = FText::FromString(TEXT("존재하지 않는 화폐입니다."));
		break;
	case UC_CurrencyComponent::E_NotEnough:
		config.strDefaultAlertMessage = FText::FromString(TEXT("화폐가 부족합니다."));
		break;
	}
	UC_GameAlertSubsystem::pushAlertMessage_Cpp(config);
}

bool UC_CurrencyComponent::getCurrencyCount(int nCurrencyID, int& rData) const
{
	const int *pData = m_mapInventory.Find(nCurrencyID);
	if (!pData)
		return false;
	rData = *pData;
	return true;
}

bool UC_CurrencyComponent::isValueCurrency(int nCurrencyID) const
{
	if (!m_pItemDataSubsystem)
		return false;
	FS_ItemData sData{};
	m_pItemDataSubsystem->getItemDataByID(nCurrencyID, sData);
	if (m_pItemDataSubsystem->GetUniqueID() == sData.nItemID || sData.eItemType != E_EItemType::E_Currency)
	{
		pushAlertMessage(E_AlertType::E_NotCurrency);
		return false;
	}
	return true;
}

