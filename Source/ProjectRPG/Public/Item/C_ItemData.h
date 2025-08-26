#pragma once

#include "CoreMinimal.h"



UENUM(BlueprintType)
enum class E_EquipEffectType : uint8
{
	E_Weapon,
	E_Armor,
	E_Helmet,
	E_Boots,
	E_Ring01,
	E_Ring02,
	E_EquipTypeMax UMETA(Hidden)
};