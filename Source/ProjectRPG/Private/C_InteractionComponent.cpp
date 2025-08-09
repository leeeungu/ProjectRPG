#include "C_InteractionComponent.h"

UC_InteractionComponent::UC_InteractionComponent() : 
	UCapsuleComponent{}
{
	SetCollisionProfileName(*getInteractionPresetName());
	CapsuleRadius = 200.0f;
	CapsuleHalfHeight = 200.0f;
}

bool UC_InteractionComponent::runInteraction(AActor* pDetectedActor)
{
	if (!pDetectedActor)
		return false;

	if (m_onInteraction.IsBound())
		m_onInteraction.Broadcast(pDetectedActor);

	return true;
}

void UC_InteractionComponent::BeginPlay()
{
	UCapsuleComponent::BeginPlay();
}
