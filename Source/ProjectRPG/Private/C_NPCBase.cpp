#include "C_NPCBase.h"
#include <C_InteractionComponent.h>

AC_NPCBase::AC_NPCBase() : ACharacter{}
{
	PrimaryActorTick.bCanEverTick = false;
	m_pInteractionComponent = CreateDefaultSubobject<UC_InteractionComponent>("InteractionComponent");
	m_pInteractionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

//bool AC_NPCBase::beginSearchingEvent_Implementation(AActor* pOther)
//{
//	return false;
//}
//
//bool AC_NPCBase::endSearchingEvent_Implementation(AActor* pOther)
//{
//	return false;
//}
//
//bool AC_NPCBase::runEvent_Implementation(AActor* pOther)
//{
//	return false;
//}
//
//bool AC_NPCBase::endEvent_Implementation()
//{
//	return false;
//}

void AC_NPCBase::BeginPlay()
{
	ACharacter::BeginPlay();
}
