#include "C_GameAlertSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UC_GameAlertSubsystem* UC_GameAlertSubsystem::m_pSingletonInstance{};


UC_GameAlertSubsystem::UC_GameAlertSubsystem()
{
}

void UC_GameAlertSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UGameInstanceSubsystem::Initialize(Collection);
	if (!m_pSingletonInstance)
		m_pSingletonInstance = this;
}

void UC_GameAlertSubsystem::Deinitialize()
{
	UGameInstanceSubsystem::Deinitialize();
	if (m_pSingletonInstance)
		m_pSingletonInstance = nullptr;
}

void UC_GameAlertSubsystem::pushAlertMessage(const FS_GameAlertSubsystemConfig& sMesssage)
{
	if (sMesssage.strDefaultAlertMessage.IsEmpty() || !GetWorld())
		return;
	if (m_nQueSize <= m_queMessage.size())
	{
		broadcastAlertMessage();
	}
	m_queMessage.push(sMesssage);
	if (!m_sTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(m_sTimerHandle, this, &UC_GameAlertSubsystem::broadcastAlertMessage, 0.1f);
	}
}

void UC_GameAlertSubsystem::pushAlertMessage_Cpp(const FS_GameAlertSubsystemConfig& sMesssage)
{
	m_pSingletonInstance->pushAlertMessage(sMesssage);
}

void UC_GameAlertSubsystem::broadcastAlertMessage()
{
	if (m_queMessage.empty() || !GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(m_sTimerHandle);
		return;
	}
	FS_GameAlertSubsystemConfig sMessage = m_queMessage.front();
	m_queMessage.pop();
	if (sMessage.bShowAlertOnScreen)
	{
		if (m_onAlertScrrenMessage.IsBound())
			m_onAlertScrrenMessage.Broadcast(sMessage);
	}
	else
	{
		if (m_onAlertDebugMessage.IsBound())
			m_onAlertDebugMessage.Broadcast(sMessage);
	}
	if (m_onAlertMessage.IsBound())
		m_onAlertMessage.Broadcast(sMessage);
	
	GetWorld()->GetTimerManager().SetTimer(m_sTimerHandle, this, &UC_GameAlertSubsystem::broadcastAlertMessage, sMessage.fDefaultAlertDuration, false);
	// todo : que 의 사이즈 등록 / 너무 많으면 큐를 비우는 로직을 추가해야함
}
