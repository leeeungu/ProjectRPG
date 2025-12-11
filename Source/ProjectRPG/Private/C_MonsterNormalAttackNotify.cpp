// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MonsterNormalAttackNotify.h"
#include "C_BaseCharacter.h"
#include "Debug/DebugDrawService.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "CPP_Player/C_Player.h"

void UC_MonsterNormalAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp->GetOwner())
		return;

	if (m_bDrawDebug)
	{
		const FTransform meshTransform = MeshComp->GetSocketTransform(m_SocketName);
		DrawDebugSphere(MeshComp->GetWorld(), meshTransform.TransformPosition(m_sRelativePosition), m_Radius, 30, FColor::Green, false, 1.f);
	}

	AC_BaseCharacter* Character = Cast<AC_BaseCharacter>(MeshComp->GetOwner());
	if (!Character)
		return;

	const FTransform MeshTransform = MeshComp->GetSocketTransform(m_SocketName);
	FVector Position = MeshTransform.TransformPosition(m_sRelativePosition);
	TArray<FHitResult> OutHits{};
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(m_Radius);
	FCollisionQueryParams Params{};
	Params.AddIgnoredActor(Character);

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
		}
	}


		
		
}
