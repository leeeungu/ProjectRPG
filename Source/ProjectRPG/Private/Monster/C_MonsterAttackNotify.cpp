#include "Monster/C_MonsterAttackNotify.h"
#include "C_BaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Debug/DebugDrawService.h"
#include "CPP_Player/C_Player.h"

void UC_MonsterAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UAnimNotify::Notify(MeshComp, Animation, EventReference);
	if (!MeshComp->GetOwner())
		return; 
	
	if (m_bDrawDebug)
	{
		const FTransform MeshTransform = MeshComp->GetSocketTransform(m_SocketName);
		DrawDebugSphere(MeshComp->GetWorld(), MeshTransform.TransformPosition(m_sRelativePosition), m_Radius, 30, FColor::Purple, false, 1.0f);
	}
	AC_BaseCharacter* Character = Cast<AC_BaseCharacter>(MeshComp->GetOwner());
	if (!Character)
	{
		if (!Character)
			UE_LOG(LogTemp, Error, TEXT("UC_AttackRayCasting::attackSphereTrace : pSelf is null"));
		if (!Cast<AC_BaseCharacter>(Character))
			UE_LOG(LogTemp, Error, TEXT("UC_AttackRayCasting::attackSphereTrace : pSelf not AC_BaseCharacter"));
		return ;
	}
	const FTransform MeshTransform = MeshComp->GetSocketTransform(m_SocketName);
	FVector Position = MeshTransform.TransformPosition(m_sRelativePosition);
	TArray<FHitResult> OutHits{};
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(m_Radius);
	FCollisionQueryParams Params{};
	Params.AddIgnoredActor(Character);
	//UE_LOG(AttackBFL, Error, TEXT("UC_AttackRayCasting::attackSphereTrace : AC_BaseCharacter"));

	Character->GetWorld()->SweepMultiByChannel(OutHits, Position, Position, FQuat::Identity, ECollisionChannel::ECC_Pawn
		, CollisionShape, Params);
	float Damage = m_Damage * Character->getAtk();
	for (FHitResult& Hit : OutHits)
	{
		UE_LOG(LogTemp, Error, TEXT("Check"));
		AC_Player* pCharacter = Cast< AC_Player>(Hit.GetActor());
		if (pCharacter)
		{
			UE_LOG(LogTemp, Error, TEXT("Attack"));
			pCharacter->takeDamageEvent(Damage);
			if (m_bSetInstigator)
				pCharacter->OnMonsterDownAttack(Hit, Character);
			else
				pCharacter->OnMonsterDownAttack(Hit, nullptr);
		}
	}
	//const FTransform MeshTransform = MeshComp->GetSocketTransform(SocketName);
	//ReturnComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(MeshComp->GetWorld(), Template, MeshTransform.TransformPosition(LocationOffset), (MeshTransform.GetRotation() * RotationOffsetQuat).Rotator(), FVector(1.0f), true);
	//}
}
