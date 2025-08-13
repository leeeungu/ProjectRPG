// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MonsterBaseCharacter.h"
#include "C_StaggerComponent.h"
#include "C_PhaseComponent.h"
#include "C_CounterComponent.h"
#include "C_MonsterAiController.h"
#include "AIController.h"

AC_MonsterBaseCharacter::AC_MonsterBaseCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AC_MonsterAiController::StaticClass();
	SetActorTickEnabled(false);
}

void AC_MonsterBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_MonsterBaseCharacter::playStaggerMontage()
{
	GetMesh()->GetAnimInstance()->Montage_Play(m_pStaggerMontage);
}

void AC_MonsterBaseCharacter::takeStaggerEvent(float fStagger)
{
	if (m_pStaggerComp)
		m_pStaggerComp->applyStagger(fStagger);

}

void AC_MonsterBaseCharacter::stopAi()
{
	AAIController* pAiCon = Cast<AAIController>(GetController());

	if (pAiCon)
	{
		pAiCon->StopMovement();

		if (UBehaviorTreeComponent* pBtComp = Cast<UBehaviorTreeComponent>(pAiCon->BrainComponent))
		{
			pBtComp->StopTree(EBTStopMode::Safe);
		}
	}
}

void AC_MonsterBaseCharacter::onStaggerBroken()
{
	AAIController* pAiCon = Cast<AAIController>(GetController());

	stopAi();

	playStaggerMontage();
}

void AC_MonsterBaseCharacter::onStaggerRecover()
{
	AC_MonsterAiController* pAiCon = Cast<AC_MonsterAiController>(GetController());

	if (pAiCon)
		pAiCon->restartAi();

	GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, m_pStaggerMontage);

	UE_LOG(LogTemp, Warning, TEXT("Recover!!!!!!!!!!!!!!!!!!!!!!!!!!"));
}

void AC_MonsterBaseCharacter::onCounterSuccess()
{

	onStaggerBroken();

	FTimerHandle sCounterEndHandle;
	GetWorld()->GetTimerManager().SetTimer(sCounterEndHandle, this,
		&AC_MonsterBaseCharacter::onStaggerRecover, 10.f, false);
}

void AC_MonsterBaseCharacter::onCounterFailed()
{
	UE_LOG(LogTemp, Warning, TEXT("Counter Failed!!!!!!!!"));
}

void AC_MonsterBaseCharacter::onDead()
{
	SetActorTickEnabled(true);
}

TArray<int32> AC_MonsterBaseCharacter::filterAvailablePatterns()
{
	TArray<int32> arrResult;
	const float fCurrentTime = GetWorld()->GetTimeSeconds();
	const float fDistToTarget = getDistanceToTarget();

	for (int32 i = 0; i < m_arrPatternList.Num(); i++)
	{
		const FS_PatternData& sPattern = m_arrPatternList[i];

		bool bCoolTimeReady = (fCurrentTime - sPattern.LastUsedTime) >= sPattern.fCoolTime;
		bool bInAttackRange = fDistToTarget <= sPattern.fAttackRange;

		if (bCoolTimeReady && bInAttackRange)
			arrResult.Add(i);
	}
	return arrResult;
}

int32 AC_MonsterBaseCharacter::selectPatternByWeight(const TArray<int32>& arrCandidates)
{
	if (arrCandidates.Num() == 0)
		return INDEX_NONE;

	int32 nTotalWeight = 0;
	for (int32 nIndex : arrCandidates)
	{
		nTotalWeight += m_arrPatternList[nIndex].nWeight;
	}

	int32 nRan = FMath::RandRange(1, nTotalWeight);
	int32 nAccWeight = 0;

	for (int32 nIndex : arrCandidates)
	{
		nAccWeight += m_arrPatternList[nIndex].nWeight;
		if (nRan <= nAccWeight)
			return nIndex;
	}
	return arrCandidates[0];
}

void AC_MonsterBaseCharacter::playPattern(int32 nPatternIndex)
{
	if (m_bIsAttacking)
		return;


	m_bIsAttacking = true;
	if (!m_arrPatternList.IsValidIndex(nPatternIndex))
		return;

	FS_PatternData& sPattern = m_arrPatternList[nPatternIndex];

	
	PlayAnimMontage(sPattern.pAttackMontage);
		
	

	float fAnimDuration = sPattern.pAttackMontage->GetPlayLength();

	FTimerHandle sAttackEndHandle;
	GetWorld()->GetTimerManager().SetTimer(sAttackEndHandle, this,
		&AC_MonsterBaseCharacter::onAttackEnd,fAnimDuration, false);

	sPattern.LastUsedTime = GetWorld()->GetTimeSeconds();
}

float AC_MonsterBaseCharacter::getDistanceToTarget() const
{
	AAIController* pAiCon = Cast<AAIController>(GetController());
	if (!pAiCon)
		return MAX_FLT;

	UBlackboardComponent* pBbCom = pAiCon->GetBlackboardComponent();
	if (!pBbCom)
		return MAX_FLT;

	AActor* pTarget = Cast<AActor>(pBbCom->GetValueAsObject(TEXT("TargetActor")));
	if (!pTarget)
		return MAX_FLT;

	return FVector::Dist(GetActorLocation(), pTarget->GetActorLocation());
}

void AC_MonsterBaseCharacter::onAttackEnd()
{
	m_bIsAttacking = false;
}

void AC_MonsterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (m_eMonsterRank >= E_MonsterRank::Named)
	{
		m_pStaggerComp = FindComponentByClass<UC_StaggerComponent>();

		m_pPhaseComp = FindComponentByClass<UC_PhaseComponent>();

		m_pCounterComp = FindComponentByClass<UC_CounterComponent>();

		if (m_pStaggerComp)
		{
			m_pStaggerComp->m_onBroken.AddDynamic(this, &AC_MonsterBaseCharacter::onStaggerBroken);

			m_pStaggerComp->m_onRecover.AddDynamic(this, &AC_MonsterBaseCharacter::onStaggerRecover);
		}

		if (m_pCounterComp)
		{
			m_pCounterComp->m_onCounterSuccess.AddDynamic(this, &AC_MonsterBaseCharacter::onCounterSuccess);

			m_pCounterComp->m_onCounterFailed.AddDynamic(this, &AC_MonsterBaseCharacter::onCounterFailed);
		}
	}

	m_onDead.AddDynamic(this, &AC_MonsterBaseCharacter::onDead);
}
