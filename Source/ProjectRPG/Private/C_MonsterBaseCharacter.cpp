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
#include "Kismet/GameplayStatics.h"
#include "../Public/Monster/C_GimmickStartPos.h"

DEFINE_LOG_CATEGORY_STATIC(C_MonsterBaseCharacte, Log, All);

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

void AC_MonsterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (m_eMonsterRank >= E_MonsterRank::Named)
	{
		m_pStaggerComp = FindComponentByClass<UC_StaggerComponent>();

		m_pStaggerGimmickComp = FindComponentByClass<UC_StaggerGimmickComponent>();


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

		m_pPhaseComp = FindComponentByClass<UC_PhaseComponent>();

	}

	m_onDead.AddDynamic(this, &AC_MonsterBaseCharacter::onDead);

}

void AC_MonsterBaseCharacter::playStaggerMontage()
{
	GetMesh()->GetAnimInstance()->Montage_Play(m_pStaggerMontage);
}

bool AC_MonsterBaseCharacter::getIsAttacking() const
{
	return m_bIsAttacking;
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

	if (m_pStaggerGimmickComp && m_pStaggerGimmickComp->getGimmickTime() >= 0.01f)
	{
		m_pStaggerGimmickComp->m_onStaggerGimmickEnd.Broadcast();
		
	}
	
	
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

	FVector vFowardOffset = GetActorForwardVector() * sPattern.fAttackRange * 1.f;
	FVector vDecalLocation = vFowardOffset + GetActorLocation();
	vDecalLocation.Z = 0.1f;
	FRotator rRot = FRotator(-90.f, 0.f, 0.f);

	
	UC_NiagaraUtil::spawnNiagaraAtLocation(GetWorld(), sPattern.pNiagara, vDecalLocation, rRot,
	sPattern.fNiagaraLife, sPattern.fNiagaraScale);

	UE_LOG(C_MonsterBaseCharacte, Warning, TEXT("Spawn Niagara at Time: %f"), GetWorld()->GetTimeSeconds());

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

	AActor* pTarget = Cast<AActor>(pBbCom->GetValueAsObject(AC_MonsterAiController::TargetActorKey));
	if (!pTarget)
		return MAX_FLT;

	return FVector::Dist(GetActorLocation(), pTarget->GetActorLocation());
}

void AC_MonsterBaseCharacter::onAttackEnd()
{
	m_bIsAttacking = false;
}

void AC_MonsterBaseCharacter::onMontageEnded_moveToGimmick(UAnimMontage* Montage, bool bInterrupted)
{
	GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &AC_MonsterBaseCharacter::onMontageEnded_moveToGimmick);

	moveToGimmick();
}

FVector AC_MonsterBaseCharacter::getGimmickPos()
{
	TArray<AActor*> arrFound{};
	FVector vGimmickPos{};

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_GimmickStartPos::StaticClass(), arrFound);

	if (arrFound.Num() > 0)
	{
		AActor* pFoundActor = arrFound[0];
		FVector vFoundPos = pFoundActor->GetActorLocation();
		vGimmickPos = vFoundPos;
	}

	return vGimmickPos;
}

void AC_MonsterBaseCharacter::moveToGimmick()
{
	SetActorLocation(getGimmickPos());
	SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));
	stopAi();
	m_bIsGimmickReady = true;
}

void AC_MonsterBaseCharacter::startGimmick()
{
	UAnimInstance* pAnim = GetMesh()->GetAnimInstance();

	if (pAnim->IsAnyMontagePlaying())
	{
		pAnim->OnMontageEnded.AddDynamic(this, &AC_MonsterBaseCharacter::onMontageEnded_moveToGimmick);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("AnyMontagePlaying!!!!!!!!!!"));
	}
	else
	{
		moveToGimmick();
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Stagger Gimmick Start!!!!!!!!!!"));
	}

}

void AC_MonsterBaseCharacter::playStaggerGimmick()
{
	startGimmick();


	/*
	* 기믹 전용 무력화 수치와 그로기 시간을 지정
	*/

	if (m_pStaggerComp)
	{
		m_fKeepMaxStagger = m_pStaggerComp->getMaxStaggerPoint();
		m_fKeepStagger = m_pStaggerComp->getCurrentStaggerPoint();
		m_fKeepBreak = m_pStaggerComp->getCurrentBreakPoint();

		if (m_pStaggerGimmickComp)
		{
			float fGoalStagger = m_pStaggerGimmickComp->getGoalStagger();
			float fGoalBreak = m_pStaggerGimmickComp->getBrokenDuration();

			m_pStaggerComp->setMaxStaggerPoint(fGoalStagger);
			m_pStaggerComp->setBreakDuration(fGoalBreak);
		}

	}
	UE_LOG(LogTemp, Error, TEXT("MaxStagger :  %.f"), m_fKeepMaxStagger);
	UE_LOG(LogTemp, Error, TEXT("CurrentStagger :  %.f"), m_fKeepStagger);

	/*
	* 무력화를 방해시킬 공격
	*/

	FVector vFowardOffset = GetActorForwardVector() * 500.f;
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
	}
	

	
}


void AC_MonsterBaseCharacter::endStaggerGimmick()
{
	if (m_pStaggerComp)
	{
		m_pStaggerComp->setMaxStaggerPoint(m_fKeepMaxStagger);
		m_pStaggerComp->setStaggerPoint(m_fKeepStagger);
		m_pStaggerComp->setBreakDuration(m_fKeepBreak);

		m_fKeepMaxStagger = 0.0f;
		m_fKeepStagger = 0.0f;
		m_fKeepBreak = 0.0f;


		UE_LOG(LogTemp, Error, TEXT("MaxStagger :  %.f"), m_pStaggerComp->getMaxStaggerPoint());
		UE_LOG(LogTemp, Error, TEXT("CurrentStagger :  %.f"), m_pStaggerComp->getCurrentStaggerPoint());
		
	}

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
