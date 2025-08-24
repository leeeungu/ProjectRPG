#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "C_AttackRayCasting.generated.h"

UCLASS()
class PROJECTRPG_API UC_AttackRayCasting : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	* @brief
	* BaseCharacter들의 takeDamageEvent 호출
	*
	* @details
	* Sphere Cast의 Pawn Object 타입으로 검색하여 자신을 제외한 AC_BaseCharacter들을 찾습니다.
	* 찾은  Character에게 Damage 만큼 takeDamageEvent을 호출합니다.
	*
	* @param AActor* pSelf 호출자 (AC_BaseCharacter)
	* @param FVector sPositionfloat Sphere 생성 위치
	* @param FName SocketName 매쉬 의 소캣 이름 NAME_None이면 캐릭터의 정면 방향
	* @param float Radius SphereTrace 반지름
	* @param float Damage
	* @param bool bDrawDebug 보라색 Debug용 콜리전 생성 여부
	* @return trace 이후 AC_BaseCharacter Detect 여부
	*/
	UFUNCTION(BlueprintCallable, Category = "Attack")
	static bool attackSphereTrace(AActor* pSelf, FVector sPositionfloat, FName SocketName = NAME_None, float Radius = 100.0f, float Damage = 1.0f, bool bDrawDebug = false);
};