#include "Notify/C_SphereAttackNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "BlueprintFunctionLibrary/C_AttackRayCasting.h"
#include "C_BaseCharacter.h"

void UC_SphereAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UAnimNotify::Notify(MeshComp, Animation, EventReference);
	UC_AttackRayCasting::attackSphereTrace(MeshComp->GetOwner(), m_sPositionfloat, m_strSocketName, m_fRadius, m_fDamage,
		m_bDrawDebug);
}
