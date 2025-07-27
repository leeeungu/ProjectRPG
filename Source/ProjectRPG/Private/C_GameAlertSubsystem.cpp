#include "C_GameAlertSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UC_GameAlertSubsystem::UC_GameAlertSubsystem()
{
}

void UC_GameAlertSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UGameInstanceSubsystem::Initialize(Collection);
}

void UC_GameAlertSubsystem::Deinitialize()
{
	UGameInstanceSubsystem::Deinitialize();
}

void UC_GameAlertSubsystem::pushAlertMessage(const FS_GameAlertSubsystemConfig& sMesssage)
{
	if (sMesssage.strDefaultAlertMessage.IsEmpty() || !GetWorld())
		return;

	m_queMessage.push(sMesssage);
	if (!m_sTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(m_sTimerHandle, this, &UC_GameAlertSubsystem::broadcastAlertMessage, 0.1f);
	}
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
		OnAlertScrrenMessage.Broadcast(sMessage);
	}
	else
	{
		OnAlertDebugMessage.Broadcast(sMessage);
	}
	
	GetWorld()->GetTimerManager().SetTimer(m_sTimerHandle, this, &UC_GameAlertSubsystem::broadcastAlertMessage, sMessage.fDefaultAlertDuration, false);
	// todo : que 의 사이즈 등록 / 너무 많으면 큐를 비우는 로직을 추가해야함
}
