#include "C_NPCBase.h"
#include <C_InteractionComponent.h>

AC_NPCBase::AC_NPCBase() : ACharacter{}
{
	PrimaryActorTick.bCanEverTick = false;
	m_pInteractionComponent = CreateDefaultSubobject<UC_InteractionComponent>("InteractionComponent");
	m_pInteractionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AC_NPCBase::BeginPlay()
{
	ACharacter::BeginPlay();
}
