#include "BlueprintFunctionLibrary/C_AttackRayCasting.h"
#include "Debug/DebugDrawService.h"
#include "C_BaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"

DEFINE_LOG_CATEGORY_STATIC(AttackBFL, Log, All);

bool UC_AttackRayCasting::attackSphereTrace(AActor* pSelf, FVector sPositionfloat, FName SocketName, float Radius, float Damage, bool bDrawDebug)
{
	if (!pSelf || !pSelf->GetWorld() || !Cast<AC_BaseCharacter>(pSelf))
	{
		if (!pSelf)
			UE_LOG(AttackBFL, Error, TEXT("UC_AttackRayCasting::attackSphereTrace : pSelf is null"));
		if (!Cast<AC_BaseCharacter>(pSelf))
			UE_LOG(AttackBFL, Error, TEXT("UC_AttackRayCasting::attackSphereTrace : pSelf not AC_BaseCharacter"));
		return false;
	}
	AC_BaseCharacter* Character = Cast<AC_BaseCharacter>(pSelf);
	FVector Position = sPositionfloat;
	Position += Character->GetMesh()->GetSocketByName(SocketName)->GetSocketLocation(Character->GetMesh());


	TArray<FHitResult> OutHits{};
	FCollisionObjectQueryParams ObjectQueryParams{};
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
	FCollisionQueryParams Params{};
	Params.AddIgnoredActor(pSelf);

	pSelf->GetWorld()->SweepMultiByObjectType(OutHits, Position, Position, FQuat::Identity, ObjectQueryParams
		, CollisionShape, Params);
	if (bDrawDebug)
	{
		DrawDebugSphere(pSelf->GetWorld(), Position, Radius, 0, FColor::Purple, false, 1.0f);
	}
	bool bResult{};
	for (FHitResult& Hit : OutHits)
	{
		AC_BaseCharacter* pCharacter = Cast< AC_BaseCharacter>(Hit.GetActor());
		if (pCharacter)
		{
			bResult = true;
			pCharacter->takeDamageEvent(Damage);
		}
	}
	return bResult;
}
