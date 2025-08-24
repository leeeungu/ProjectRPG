#include "Notify/C_SphereAttackNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "BlueprintFunctionLibrary/C_AttackRayCasting.h"
#include "C_BaseCharacter.h"
#include "Debug/DebugDrawService.h"
#include "Engine/SkeletalMeshSocket.h"

DEFINE_LOG_CATEGORY_STATIC(C_SphereAttackNotify, Log, All);

void UC_SphereAttackNotify::OnAnimNotifyCreatedInEditor(FAnimNotifyEvent& ContainingAnimNotifyEvent)
{
	//ContainingAnimNotifyEvent.
}

void UC_SphereAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	UAnimNotify::Notify(MeshComp, Animation, EventReference);
#if WITH_EDITOR
	if (m_bDrawDebug && MeshComp && MeshComp->GetSocketByName(m_strSocketName))
	{
		FVector Position = m_sPosition;
		Position += MeshComp->GetSocketByName(m_strSocketName)->GetSocketLocation(MeshComp);
		if (GetWorld())
			DrawDebugSphere(GetWorld(), m_sPosition, m_fRadius, 0, FColor::Purple, false, 1.0f);
	}
#elif
	UC_AttackRayCasting::attackSphereTrace(MeshComp->GetOwner(), m_sPosition, m_strSocketName, m_fRadius, m_fDamage, m_bDrawDebug);
#endif
}
