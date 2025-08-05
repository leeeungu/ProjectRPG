#include "C_AnimationInteraction.h"
#include <GameFramework/Character.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Engine/TargetPoint.h>
#include <Animation/AnimInstance.h>
#include <C_InteractionComponent.h>

AC_AnimationInteraction::AC_AnimationInteraction() :
	AActor{}
{
	m_pRoot = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(m_pRoot);
	PrimaryActorTick.bCanEverTick = true;
	m_pInteractionCollision = CreateDefaultSubobject< UC_InteractionComponent>("Detector");
	m_pInteractionCollision->AttachToComponent(m_pRoot, FAttachmentTransformRules::KeepRelativeTransform);
	m_pStartDirection = CreateDefaultSubobject< UArrowComponent>("Direction");
	m_pStartDirection->AttachToComponent(m_pRoot, FAttachmentTransformRules::KeepRelativeTransform);
}

void AC_AnimationInteraction::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
	//float fDeltaSpeed = DeltaTime * m_fLoopMoveSpeed;
	//if (m_fDistance < fDeltaSpeed)
	//	fDeltaSpeed = m_fDistance;
	//m_fDistance -= fDeltaSpeed;
	//FVector location = m_pDetector->GetActorLocation() + (m_sDirection* fDeltaSpeed);
	//	m_pDetector->SetActorLocation(location);
}

void AC_AnimationInteraction::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	AActor::PostEditChangeProperty(PropertyChangedEvent);
	FName PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	FS_InteractionAnimationData* pData{};
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AC_AnimationInteraction, m_sStartAnimation))
	{
		pData = &m_sStartAnimation;
		setAnimationData(pData);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AC_AnimationInteraction, m_sEndAnimation))
	{
		pData = &m_sEndAnimation;
		setAnimationData(pData);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AC_AnimationInteraction, m_sLoopAnimation))
	{
		pData = &m_sLoopAnimation;
		setAnimationData(pData);
	}
}

void AC_AnimationInteraction::BeginPlay()
{
	SetActorTickEnabled(false);
	AActor::BeginPlay();
	if (!m_pInteractionCollision)
		return;
	m_pInteractionCollision->m_onInteraction.AddDynamic(this, &AC_AnimationInteraction::playAnimation);
	setAnimationData(&m_sStartAnimation);
	setAnimationData(&m_sEndAnimation);
	
	FS_InteractionAnimationData* pData = &m_sLoopAnimation;
	if (pData->pPlayMontage)
	{
		if (!FMath::IsNearlyZero(pData->pPlayMontage->GetPlayLength()))
		{
			pData->fMoveSpeed = (pData->endLocation - pData->startLocation).Length() / pData->pPlayMontage->GetPlayLength();
		}
		if (m_pEndPoint && !FMath::IsNearlyZero(pData->fMoveSpeed) && pData->fMoveSpeed > 0.f)
		{
			float Distance = (GetActorLocation() - m_pEndPoint->GetActorLocation()).Length();
			Distance -= m_sStartAnimation.fDistance + m_sEndAnimation.fDistance;
			pData->fNextMontageTime = Distance / pData->fMoveSpeed;
		}
	}
}

void AC_AnimationInteraction::playAnimation(AActor* pDetectedActor)
{
	if (!pDetectedActor || !m_sStartAnimation.pPlayMontage)
		return;
	m_pDetector = Cast<ACharacter>(pDetectedActor);
	if (m_pDetector && m_pDetector->GetMesh())
		m_pInstance = m_pDetector->GetMesh()->GetAnimInstance();
	if (!m_pDetector || !m_pInstance)
		return;
	if (m_pDetector->GetInstigatorController())
		m_pDetector->GetInstigatorController()->SetActorTickEnabled(false);
	m_pDetector->SetActorRotation(m_pStartDirection->GetComponentRotation());
	m_pDetector->SetActorLocation(m_pStartDirection->GetComponentLocation());
	m_pDetector->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	m_pInstance->Montage_Play(m_sStartAnimation.pPlayMontage);
	FTimerHandle handle{};
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AC_AnimationInteraction::playLoopAnimation, m_sStartAnimation.pPlayMontage->GetPlayLength()-0.01f);
}

void AC_AnimationInteraction::playLoopAnimation()
{
	if (!m_pDetector || !m_pInstance || !m_sLoopAnimation.pPlayMontage)
		return;
	m_pInstance->Montage_Play(m_sLoopAnimation.pPlayMontage);
	FTimerHandle handle{};
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AC_AnimationInteraction::playEndAnimation, m_sLoopAnimation.fNextMontageTime);
}

void AC_AnimationInteraction::playEndAnimation()
{
	if (!m_pDetector || !m_pInstance || !m_sEndAnimation.pPlayMontage)
		return;
	m_pInstance->Montage_Play(m_sEndAnimation.pPlayMontage);
	FTimerHandle handle{};
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AC_AnimationInteraction::resetAnimation, m_sEndAnimation.pPlayMontage->GetPlayLength());
}

void AC_AnimationInteraction::resetAnimation()
{
	m_pDetector->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	if (m_pDetector->GetInstigatorController())
		m_pDetector->GetInstigatorController()->SetActorTickEnabled(true);
	m_pDetector = nullptr;
	m_pInstance = nullptr;
}

void AC_AnimationInteraction::setAnimationData(FS_InteractionAnimationData* pData)
{
	if (pData && pData->pPlayMontage)
	{
		if (!FMath::IsNearlyZero(pData->pPlayMontage->GetPlayLength()))
		{
			pData->fDistance = (pData->endLocation - pData->startLocation).Length();
			pData->fMoveSpeed = pData->fDistance / pData->pPlayMontage->GetPlayLength();
		}
		if (m_pEndPoint && !FMath::IsNearlyZero(pData->fMoveSpeed) && pData->fMoveSpeed > 0.f)
		{
			pData->fNextMontageTime = (GetActorLocation() - m_pEndPoint->GetActorLocation()).Length() / pData->fMoveSpeed;
		}
	}
}
