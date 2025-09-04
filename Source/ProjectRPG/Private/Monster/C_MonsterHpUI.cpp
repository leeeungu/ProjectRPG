// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/C_MonsterHpUI.h"
#include "Components/ProgressBar.h"

void UC_MonsterHpUI::updateHpBars(float fCurHp, float fMaxHp)
{
	if (m_arrHpBars.Num() == 0)
		return;

	/*
	* 전체 체력을 한 구간 체력으로 나누면 몇개의 구간으로 나뉘는지 계산
	* 소수점이 있으면 올림
	*/
	int32 nNumSegments = FMath::CeilToInt(fMaxHp / m_fSegMentHp);

	/*
	* 프로그래스바 개수가 부족할경우
	* 가능한 만큼만 진행
	*/
	if (m_arrHpBars.Num() < nNumSegments)
	{
		nNumSegments = m_arrHpBars.Num();
	}

	for (int32 i = 0; i < nNumSegments; ++i)
	{
		float fSegmentStartHp = i * m_fSegMentHp;
		float fSegmentEndHp = fSegmentStartHp + m_fSegMentHp;

		float fHpInSegment = FMath::Clamp(fCurHp - fSegmentStartHp, 0.f, m_fSegMentHp);
		float fPercent = fHpInSegment / m_fSegMentHp;

		m_arrHpBars[i]->SetPercent(fPercent);

		m_arrHpBars[i]->SetRenderOpacity(fPercent > 0 ? 1.f : 0.f);
	}

	for (int32 i = nNumSegments; i < m_arrHpBars.Num(); ++i)
	{
		m_arrHpBars[i]->SetPercent(0.f);
	}
}
