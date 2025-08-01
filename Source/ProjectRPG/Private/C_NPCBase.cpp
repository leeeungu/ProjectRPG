#include "C_NPCBase.h"
#include <C_SearchingCollision.h>

AC_NPCBase::AC_NPCBase() : ACharacter{}
{
	PrimaryActorTick.bCanEverTick = false;
	m_pSearchCollision = CreateDefaultSubobject<UC_SearchingCollision>("SearchCollision");
	m_pSearchCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

bool AC_NPCBase::beginSearchingEvent_Implementation(AActor* pOther)
{
	return false;
}

bool AC_NPCBase::endSearchingEvent_Implementation(AActor* pOther)
{
	return false;
}

void AC_NPCBase::BeginPlay()
{
	ACharacter::BeginPlay();
}
