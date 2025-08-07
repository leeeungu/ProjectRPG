#include "C_AnimationInteraction.h"
#include <GameFramework/Character.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Engine/TargetPoint.h>
#include <Animation/AnimInstance.h>
#include <C_InteractionComponent.h>
#include <Kismet/KismetMathLibrary.h>

AC_AnimationInteraction::AC_AnimationInteraction() :
	AActor{}
{
	m_pRoot = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(m_pRoot);

	m_pStartCollision = CreateDefaultSubobject< UC_InteractionComponent>("StartCollision");
	m_pStartCollision->AttachToComponent(m_pRoot, FAttachmentTransformRules::KeepRelativeTransform);

	m_pStartDirection = CreateDefaultSubobject< UArrowComponent>("Direction");
	m_pStartDirection->AttachToComponent(m_pRoot, FAttachmentTransformRules::KeepRelativeTransform);

	m_pEndCollision = CreateDefaultSubobject< UC_InteractionComponent>("EndCollision");
}

void AC_AnimationInteraction::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	AActor::PostEditChangeProperty(PropertyChangedEvent);
	if (m_bLookEndCollision)
	{
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(m_pStartDirection->GetComponentLocation(), m_pEndCollision->GetComponentLocation());
		Rot.Roll = 0.0;
		Rot.Pitch = 0.0;
		m_pStartDirection->SetWorldRotation(Rot);
	}
}

void AC_AnimationInteraction::PostEditMove(bool bFinished)
{
	AActor::PostEditMove(bFinished);
	if (m_bLookEndCollision)
	{
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(m_pStartDirection->GetComponentLocation(), m_pEndCollision->GetComponentLocation());

		Rot.Roll = 0.0;
		Rot.Pitch = 0.0;
		m_pStartDirection->SetWorldRotation(Rot);
	}
}

void AC_AnimationInteraction::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}

void AC_AnimationInteraction::BeginPlay()
{
	SetActorTickEnabled(false);
	AActor::BeginPlay();
	m_pStartCollision->m_onInteraction.AddDynamic(this, &AC_AnimationInteraction::playAnimation);
	m_pEndCollision->OnComponentBeginOverlap.AddDynamic(this, &AC_AnimationInteraction::beginEndCollision);
}

void AC_AnimationInteraction::playAnimation(AActor* pDetectedActor)
{
	if (!pDetectedActor || m_eStartType == E_TrabelType::E_NONE || m_bPlay)
		return;
	m_pDetector = Cast<ACharacter>(pDetectedActor);
	m_pTravelManagerComponent = pDetectedActor->GetComponentByClass<UC_TravelManagerComponent>();
	if (!m_pDetector || !m_pTravelManagerComponent)
		return;
	m_pDetector->SetActorRotation(m_pStartDirection->GetComponentRotation());
	m_pDetector->SetActorLocation(m_pStartDirection->GetComponentLocation());
	m_pTravelManagerComponent->setTravelType(m_eStartType);
	m_bPlay = true;
}

void AC_AnimationInteraction::beginEndCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!m_pDetector || m_pDetector != Cast<ACharacter>(OtherActor) || m_eEndType == E_TrabelType::E_NONE || !m_pTravelManagerComponent || !m_bPlay)
		return;
	m_pTravelManagerComponent->setTravelType(m_eEndType);
	m_pDetector = nullptr;
	m_pTravelManagerComponent = nullptr;
	m_bPlay = false;
}