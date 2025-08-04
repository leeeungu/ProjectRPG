#include "C_AnimationInteraction.h"
#include <GameFramework/Character.h>
#include <Components/SkeletalMeshComponent.h>
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
}

void AC_AnimationInteraction::Tick(float DeltaTime)
{
	if (!m_bLoopMove || !m_pDetector)
		return;
	//AActor::Tick(DeltaTime);
	//float fDeltaSpeed = DeltaTime * m_fLoopMoveSpeed;
	//if (m_fDistance < fDeltaSpeed)
	//	fDeltaSpeed = m_fDistance;
	//m_fDistance -= fDeltaSpeed;
	//FVector location = m_pDetector->GetActorLocation() + (m_sDirection* fDeltaSpeed);
	//	m_pDetector->SetActorLocation(location);
}

void AC_AnimationInteraction::BeginPlay()
{
	SetActorTickEnabled(false);
	AActor::BeginPlay();
	if (!m_pInteractionCollision)
		return;
	m_pInteractionCollision->m_onInteraction.AddDynamic(this, &AC_AnimationInteraction::playAnimation);

	if (!m_pEndPoint)
		return;
	FVector Start = GetActorLocation();
	FVector End = m_pEndPoint->GetActorLocation();
	m_sDirection = End - Start;
	m_fLoopMoveSpeed = m_sDirection.Length() / (m_fLoopMoveSpeed / m_pLoopMontage->GetPlayLength());
	m_sRotate = m_sDirection.Rotation();
	m_sDirection.Normalize();
	m_sRotate.Roll = 0;
	m_sRotate.Pitch = 0;
}

void AC_AnimationInteraction::playAnimation(AActor* pDetectedActor)
{
	if (!pDetectedActor || !m_pStartMontage)
		return;
	m_pDetector = Cast<ACharacter>(pDetectedActor);
	if (m_pDetector && m_pDetector->GetMesh())
		m_pInstance = m_pDetector->GetMesh()->GetAnimInstance();
	if (!m_pDetector || !m_pInstance)
		return;
	FRotator Rotate = m_pDetector->GetActorRotation();
	Rotate.Yaw = m_sRotate.Yaw;
	m_pDetector->SetActorRotation(Rotate);
	m_pDetector->SetActorLocation(GetActorLocation());
	if (m_pDetector->GetInstigatorController())
		m_pDetector->GetInstigatorController()->SetActorTickEnabled(false);
	m_pInstance->Montage_Play(m_pStartMontage);
	FTimerHandle handle{};
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AC_AnimationInteraction::playLoopAnimation, m_pStartMontage->GetPlayLength());
}

void AC_AnimationInteraction::playLoopAnimation()
{
	if (!m_pDetector || !m_pInstance || !m_pLoopMontage || !m_pEndPoint)
		return;
	m_bLoopMove = true;
	SetActorTickEnabled(true);
	m_pDetector->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	m_fDistance = (m_pEndPoint->GetActorLocation() - GetActorLocation()).Length();
	m_pInstance->Montage_Play(m_pLoopMontage);
	FTimerHandle handle{};
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AC_AnimationInteraction::playEndAnimation, m_fLoopMoveSpeed);
}

void AC_AnimationInteraction::playEndAnimation()
{
	if (!m_pDetector || !m_pInstance || !m_pEndMontage)
		return;
	m_bLoopMove = false;
	SetActorTickEnabled(false);
	m_pInstance->Montage_Play(m_pEndMontage);
	FTimerHandle handle{};
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AC_AnimationInteraction::resetAnimation, m_pEndMontage->GetPlayLength());
}

void AC_AnimationInteraction::resetAnimation()
{
	m_pDetector->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	if (m_pDetector->GetInstigatorController())
		m_pDetector->GetInstigatorController()->SetActorTickEnabled(true);
	m_pDetector = nullptr;
	m_pInstance = nullptr;
}
