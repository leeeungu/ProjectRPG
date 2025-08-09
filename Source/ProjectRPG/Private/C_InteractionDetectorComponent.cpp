#include "C_InteractionDetectorComponent.h"
#include "C_InteractionComponent.h"

UC_InteractionDetectorComponent::UC_InteractionDetectorComponent() : 
	UCapsuleComponent{}
{
	SetCollisionProfileName(*getDetectorPresetName());
	CapsuleRadius = 200.0f;
	CapsuleHalfHeight = 200.0f;
}

void UC_InteractionDetectorComponent::BeginPlay()
{
	UCapsuleComponent::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UC_InteractionDetectorComponent::beginDetected);
	OnComponentEndOverlap.AddDynamic(this, &UC_InteractionDetectorComponent::endDetected);
}

bool UC_InteractionDetectorComponent::runInteraction()
{
	if (!m_pFirst)
		return false;
	m_pFirst->runInteraction(GetOwner());
	return true;
}

void UC_InteractionDetectorComponent::beginDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UC_InteractionComponent* pInteraction = Cast< UC_InteractionComponent>(OtherComp);
	if (!pInteraction)
		return;
	m_setInteractionComponents.FindOrAdd(pInteraction);
	if (!m_pFirst)
		m_pFirst = *m_setInteractionComponents.begin();
}

void UC_InteractionDetectorComponent::endDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UC_InteractionComponent* pInteraction = Cast< UC_InteractionComponent>(OtherComp);
	if (!pInteraction)
		return;
	m_setInteractionComponents.Remove(pInteraction);
	if (m_pFirst == pInteraction)
		m_pFirst = nullptr;
	if (m_pFirst == nullptr && !m_setInteractionComponents.IsEmpty())
		m_pFirst = *m_setInteractionComponents.begin();
}
