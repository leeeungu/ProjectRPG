#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "C_EquipEffectFunctionLibrary.generated.h"

class AC_BaseCharacter;

UCLASS()
class PROJECTRPG_API UC_EquipEffectFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "EquipEffectFunction")
	static void effectAttack(AC_BaseCharacter* TargetActor, float Value);
	UFUNCTION(BlueprintCallable, Category = "EquipEffectFunction")
	static void effectHealth(AC_BaseCharacter* TargetActor, float Value);
	UFUNCTION(BlueprintCallable, Category = "EquipEffectFunction")
	static void unEffectAttack(AC_BaseCharacter* TargetActor, float Value);
	UFUNCTION(BlueprintCallable, Category = "EquipEffectFunction")
	static void unEffectHealth(AC_BaseCharacter* TargetActor, float Value);
};
