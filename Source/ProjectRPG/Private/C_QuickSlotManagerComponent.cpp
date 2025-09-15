#include "C_QuickSlotManagerComponent.h"
#include <GameFramework/PlayerController.h>
#include <C_InventoryComponent.h>
#include <C_GameAlertSubsystem.h>
#include "GamePlay/C_DataMangerSubsystem.h"
#include "C_ItemActorBase.h"

UC_QuickSlotManagerComponent::UC_QuickSlotManagerComponent() :
	UActorComponent{}
{
	PrimaryComponentTick.bCanEverTick = false;
	for (int i = 0; i < (uint8)E_QuickSlotType::E_QuickSlot_MAX; i++)
	{
		m_arrQuickSlotItem[i] = UC_ItemDataSubsystem::getUnValidItemID_CPP();
	}
}

void UC_QuickSlotManagerComponent::setQuickSlotItem(E_QuickSlotType QuickSlotType, int ItemID)
{
	m_arrQuickSlotItem[(uint8)QuickSlotType] = ItemID;
	if (m_onQuickSlotChange[(uint8)QuickSlotType].IsBound())
	{
		m_onQuickSlotChange[(uint8)QuickSlotType].Execute(m_arrQuickSlotItem[(uint8)QuickSlotType]);
	}
}

bool UC_QuickSlotManagerComponent::getQuickSlotItemID(E_QuickSlotType QuickSlotType, int& useItemID) const
{
	useItemID = UC_ItemDataSubsystem::getUnValidItemID_CPP();
	if (!m_pInventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UC_QuickSlotManagerComponent::useQuickSlot - m_pInventoryComponent is null"));
		return false;
	}
	if (QuickSlotType <= E_QuickSlotType::E_None || QuickSlotType >= E_QuickSlotType::E_QuickSlot_MAX)
	{
		UE_LOG(LogTemp, Warning, TEXT("UC_QuickSlotManagerComponent::useQuickSlot - Invalid QuickSlotType: %d"), (int)QuickSlotType);
		return false;
	}

	int ItemID = m_arrQuickSlotItem[(int)QuickSlotType];
	useItemID = ItemID;

	if (useItemID == UC_ItemDataSubsystem::getUnValidItemID_CPP())
	{
		FS_GameAlertSubsystemConfig config{};
		config.strDefaultAlertMessage = FText::FromString(TEXT("퀵슬롯에 아이템이 없습니다."));
		UC_GameAlertSubsystem::pushAlertMessage_Cpp(config);
		return false;
	}

	int nCount{};
	if (!m_pInventoryComponent->getItemCountByID(useItemID, nCount) || nCount <= 0)
	{
		FS_GameAlertSubsystemConfig config{};
		config.strDefaultAlertMessage = FText::FromString(TEXT("아이템이 부족합니다."));
		UC_GameAlertSubsystem::pushAlertMessage_Cpp(config);
		return false;
	}
	return true;
}

void UC_QuickSlotManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UActorComponent::EndPlay(EndPlayReason);
	UC_DataMangerSubsystem::saveBinaryData(this);
}

int UC_QuickSlotManagerComponent::getQuickSlotID(E_QuickSlotType QuickSlotType) const
{
	return m_arrQuickSlotItem[(int)QuickSlotType];
}

void UC_QuickSlotManagerComponent::bindSlotChangeDelegate(E_QuickSlotType QuickSlotType, FOnQuickSlotChange Delegate)
{
	m_onQuickSlotChange[(int)QuickSlotType] = Delegate;
}

void UC_QuickSlotManagerComponent::useQuickSlotItemID(E_QuickSlotType QuickSlotType)
{
	int nItemID{};
	if (getQuickSlotItemID(QuickSlotType, nItemID) && Cast<APlayerController>(GetOwner()))
	{
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();
		if (GameInstance)
		{
			UC_ItemDataSubsystem* pItemDataSubsystem = GameInstance->GetSubsystem<UC_ItemDataSubsystem>();
			AC_ItemActorBase* Item = pItemDataSubsystem->spawnEffectItem(nItemID, Cast<APlayerController>(GetOwner())->AcknowledgedPawn);
			if (Item && Item->useItemActor())
			{
				m_pInventoryComponent->removeItem(nItemID, 1);
			}
		}
	}
}

void UC_QuickSlotManagerComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
	m_pInventoryComponent = GetOwner()->GetComponentByClass<UC_InventoryComponent>();
	UC_DataMangerSubsystem::loadData(this);
}

E_DataType UC_QuickSlotManagerComponent::getDataType()
{
	return E_DataType::E_Binary;
}

FString UC_QuickSlotManagerComponent::getFilePath(E_DataType eType)
{
	return  FPaths::ProjectSavedDir() + TEXT("QuickSlotData");
}

void UC_QuickSlotManagerComponent::loadBinaryData(TArray<uint8>& arData)
{
	FS_QuickSlotSaveData Data{};
	if (!UC_DataMangerSubsystem::readBinaryFile(arData, &Data))
		return;

	for (int i = 0; i < (uint8)E_QuickSlotType::E_QuickSlot_MAX; i++)
	{
		setQuickSlotItem((E_QuickSlotType)i, Data.arrQuickSlotItem[i]);
	}
}

TArray<uint8> UC_QuickSlotManagerComponent::getBinaryData()
{
	FS_QuickSlotSaveData Data{};
	for (int i = 0; i < (uint8)E_QuickSlotType::E_QuickSlot_MAX; i++)
	{
		Data.arrQuickSlotItem[i] = m_arrQuickSlotItem[i];
	}
	TArray<uint8> result{};
	UC_DataMangerSubsystem::saveBinaryFile< FS_QuickSlotSaveData>(result, &Data);
	return result;
}
