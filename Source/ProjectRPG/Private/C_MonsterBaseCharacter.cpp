// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MonsterBaseCharacter.h"
#include "C_StaggerComponent.h"
#include "C_PhaseComponent.h"
#include "C_CounterComponent.h"
#include "Monster/C_GimmickComponent.h"
#include "C_MonsterAiController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "AIController.h"
#include "C_DecalUtils.h"
#include "C_NiagaraUtil.h"
#include "../Public/Monster/C_StaggerGimmickComponent.h"
#include "GamePlay/C_DamageWidgetComponent.h"



DEFINE_LOG_CATEGORY_STATIC(C_MonsterBaseCharacte, Log, All);

AC_MonsterBaseCharacter::AC_MonsterBaseCharacter() : AC_BaseCharacter{}
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AC_MonsterAiController::StaticClass();
	SetActorTickEnabled(false);

	m_pDamageWidget = CreateDefaultSubobject< UC_DamageWidgetComponent>(TEXT("DamageWidget"));
	m_pDamageWidget->SetupAttachment(RootComponent);

}

void AC_MonsterBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_MonsterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_pAiCon = Cast<AC_MonsterAiController>(GetController());

	if (m_eMonsterRank >= E_MonsterRank::Named)
	{
		m_pStaggerComp = FindComponentByClass<UC_StaggerComponent>();

		m_pStaggerGimmickComp = FindComponentByClass<UC_StaggerGimmickComponent>();

		m_pPhaseComponent = FindComponentByClass<UC_PhaseComponent>();

		if (m_pPhaseComponent)
		{
			m_pPhaseComponent->m_onPhaseFinished.AddDynamic(this, &AC_MonsterBaseCharacter::onPhaseChangeFinished);
		}


		if (m_pStaggerGimmickComp)
		{
			m_pStaggerGimmickComp->m_onStaggerGimmickStart.AddDynamic(this, &AC_MonsterBaseCharacter::playStaggerGimmick);
			m_pStaggerGimmickComp->m_onStaggerGimmickEnd.AddDynamic(this, &AC_MonsterBaseCharacter::endStaggerGimmick);

		}

		if (m_pStaggerComp)
		{
			m_pStaggerComp->m_onBroken.AddDynamic(this, &AC_MonsterBaseCharacter::onStaggerBroken);

			m_pStaggerComp->m_onRecover.AddDynamic(this, &AC_MonsterBaseCharacter::onStaggerRecover);
		}

		m_pCounterComp = FindComponentByClass<UC_CounterComponent>();

		if (m_pCounterComp)
		{
			m_pCounterComp->m_onCounterSuccess.AddDynamic(this, &AC_MonsterBaseCharacter::onCounterSuccess);

			m_pCounterComp->m_onCounterFailed.AddDynamic(this, &AC_MonsterBaseCharacter::onCounterFailed);
		}

	}

	m_onDead.AddDynamic(this, &AC_MonsterBaseCharacter::onDead);


}

void AC_MonsterBaseCharacter::setPhaseState(E_MonsterPhaseState eState)
{
	m_eMonsterState = eState;
}

E_MonsterPhaseState AC_MonsterBaseCharacter::getCurrentState() const
{
	return m_eMonsterState;
}

void AC_MonsterBaseCharacter::playStaggerMontage()
{
	GetMesh()->GetAnimInstance()->Montage_Play(m_pStaggerMontage);
}

bool AC_MonsterBaseCharacter::getIsAttacking() const
{
	return m_eMonsterState == E_MonsterPhaseState::Attacking;
}

void AC_MonsterBaseCharacter::takeStaggerEvent(float fStagger)
{
	if (m_pStaggerComp)
		m_pStaggerComp->applyStagger(fStagger);

}

void AC_MonsterBaseCharacter::stopAi()
{
	m_pAiCon->stopAiCon();
}

void AC_MonsterBaseCharacter::reStartAi()
{
	m_pAiCon->restartAiCon();
}

void AC_MonsterBaseCharacter::onStaggerBroken()
{
	AAIController* pAiCon = Cast<AAIController>(GetController());

	setPhaseState(E_MonsterPhaseState::Stagger);

	stopAi();

	playStaggerMontage();
}

void AC_MonsterBaseCharacter::onStaggerRecover()
{
	if (m_pAiCon)
		reStartAi();


	GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, m_pStaggerMontage);

	tryTriggerPhaseChangeOrGimmick();

	UE_LOG(LogTemp, Warning, TEXT("Recover!!!!!!!!!!!!!!!!!!!!!!!!!!"));

	

	if (m_eMonsterState == E_MonsterPhaseState::GimmickExecute && m_pStaggerGimmickComp && m_pStaggerGimmickComp->getGimmickTime() >= 0.01f)
	{
		m_pStaggerGimmickComp->m_onStaggerGimmickEnd.Broadcast();
		
	}
	
	UE_LOG(LogTemp, Warning, TEXT("MaxStagger : %.f"), m_pStaggerComp->getMaxStaggerPoint());
}

void AC_MonsterBaseCharacter::onCounterSuccess()
{
	AAIController* pAiCon = Cast<AAIController>(GetController());
	if (!pAiCon)
		return;

	UBlackboardComponent* pBbCom = pAiCon->GetBlackboardComponent();
	if (!pBbCom)
		return;

	AActor* pTarget = Cast<AActor>(pBbCom->GetValueAsObject(AC_MonsterAiController::TargetActorKey));
	if (!pTarget)
		return;

	FVector vToPlayer = pTarget->GetActorLocation() - GetActorLocation();
	FRotator rLookAt = vToPlayer.Rotation();
	rLookAt.Pitch = 0.0f;
	rLookAt.Roll = 0.0f;

	SetActorRotation(rLookAt);

	GetMesh()->GetAnimInstance()->Montage_Stop(0.25f);

	float fCounterMontageDuration = m_pCounterMontage->GetPlayLength();
	GetMesh()->GetAnimInstance()->Montage_Play(m_pCounterMontage);

	FTimerHandle sCounterStartHandle;
	GetWorld()->GetTimerManager().SetTimer(sCounterStartHandle,
		FTimerDelegate::CreateLambda([this]()
			{
				onStaggerBroken();
			}), fCounterMontageDuration, false);

	FTimerHandle sCounterEndHandle;
	GetWorld()->GetTimerManager().SetTimer(sCounterEndHandle, this,
		&AC_MonsterBaseCharacter::onStaggerRecover, 10.f, false);
}

void AC_MonsterBaseCharacter::onCounterFailed()
{
	UE_LOG(LogTemp, Warning, TEXT("Counter Failed!!!!!!!!"));
}

void AC_MonsterBaseCharacter::tryCounter()
{
	if (m_pCounterComp)
	{
		m_pCounterComp->tryCounter();
	}
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
	if (!(m_eMonsterState == E_MonsterPhaseState::Idle))
		return;

	setPhaseState(E_MonsterPhaseState::Attacking);


	FS_PatternData& sPattern = m_arrPatternList[nPatternIndex];

	FVector vFowardOffset = GetActorForwardVector() * sPattern.fAttackRange * 1.f;
	FVector vDecalLocation = vFowardOffset + GetActorLocation();
	vDecalLocation.Z = 0.1f;
	FRotator rRot = FRotator(-90.f, 0.f, 0.f);


	PlayAnimMontage(sPattern.pAttackMontage);
		
	
	float fAnimDuration = sPattern.pAttackMontage->GetPlayLength() + 0.1f;

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

	AActor* pTarget = Cast<AActor>(pBbCom->GetValueAsObject(AC_MonsterAiController::TargetActorKey));
	if (!pTarget)
		return MAX_FLT;

	return FVector::Dist(GetActorLocation(), pTarget->GetActorLocation());
}

void AC_MonsterBaseCharacter::onAttackEnd()
{
	setPhaseState(E_MonsterPhaseState::Idle);
	tryTriggerPhaseChangeOrGimmick();
}

void AC_MonsterBaseCharacter::tryTriggerPhaseChangeOrGimmick()
{

	if (bPendingPhaseChange)
	{
		bPendingPhaseChange = false;

		if (m_pPhaseComponent)
		{

			m_pPhaseComponent->phaseChange(fPendingHp, fPendingMaxHp);

			return;
		}
	}

	// 기믹 예약 처리
	if (bPendingGimmickStart)
	{
		bPendingGimmickStart = false;

		if (m_pStaggerGimmickComp)
		{
			m_pStaggerGimmickComp->startGimmick();
		}
	}

	
}


void AC_MonsterBaseCharacter::playStaggerGimmick()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("playStaggerGimmick Called"));
	if (m_pStaggerGimmickComp)
	{
		m_pStaggerGimmickComp->startGimmick();

	}


	/*
	* 무력화를 방해시킬 공격
	*/

	/*FVector vFowardOffset = GetActorForwardVector() * 500.f;
	FVector vRightOffset = GetActorRightVector() * 500.f;
	FVector vDecalLocation = vFowardOffset + GetActorLocation();
	FVector vDecalLocation2 = vRightOffset + GetActorLocation();
	vDecalLocation.Z = 0.1f;
	vDecalLocation2.Z = 0.1f;

	if (m_bIsGimmickReady)
	{
		UC_NiagaraUtil::spawnNiagaraAtLocation(GetWorld(), m_pDangerPlace, vDecalLocation,
			FRotator(-90.f, 0.f, 0.f), 3.f, 1000.f);

		UC_NiagaraUtil::spawnNiagaraAtLocation(GetWorld(), m_pDangerPlace, vDecalLocation2,
			FRotator(-90.f, 0.f, 0.f), 3.f, 800.f);
	}*/
	

	
}


void AC_MonsterBaseCharacter::endStaggerGimmick()
{
	if (m_pStaggerComp && m_pStaggerGimmickComp)
	{
		m_pStaggerGimmickComp->restoreStagger(m_pStaggerComp);
	}

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("End Gimmick failed!!!!!!!!!!"));
	/*
	* 기믹 실패 처리
	* 광역 높은 데미지 등
	* 처리 후 AI 재가동 시키기
	*/
}



void AC_MonsterBaseCharacter::Destroyed()
{
	Super::Destroyed();
	m_onMonsterDied.Broadcast();
}

void AC_MonsterBaseCharacter::onPhaseChangeFinished()
{
	if (bPendingGimmickStart && m_pStaggerGimmickComp)
	{
		bPendingGimmickStart = false;
		m_pStaggerGimmickComp->startGimmick();
	}
}
