#include "Item/FunctionLibrary/C_EquipEffectFunctionLibrary.h"
#include "C_BaseCharacter.h"

void UC_EquipEffectFunctionLibrary::effectAttack(AC_BaseCharacter* TargetActor , float Value)
{
	if (!TargetActor)
		return;
	float fAtk = TargetActor->getAtk();
	fAtk = FMath::Clamp(fAtk, fAtk, TNumericLimits<float>::Max() - Value);
	fAtk += Value;
	TargetActor->setAtk(fAtk);
}

void UC_EquipEffectFunctionLibrary::effectHealth(AC_BaseCharacter* TargetActor, float Value)
{
	if (!TargetActor)
		return;
	float fHp = TargetActor->getHp();
	fHp = FMath::Clamp(fHp, fHp, TNumericLimits<float>::Max() - Value);
	fHp += Value;
	TargetActor->setHp(fHp);

	fHp = TargetActor->getMaxHp();
	fHp = FMath::Clamp(fHp, fHp, TNumericLimits<float>::Max() - Value);
	fHp += Value;
	TargetActor->setMaxHp(fHp);
}

void UC_EquipEffectFunctionLibrary::unEffectAttack(AC_BaseCharacter* TargetActor, float Value)
{
	if (!TargetActor)
		return;
	float fAtk = TargetActor->getAtk();
	fAtk = FMath::Clamp(fAtk, TNumericLimits<float>::Min() + Value, fAtk);
	fAtk -= Value;
	TargetActor->setAtk(fAtk);
}

void UC_EquipEffectFunctionLibrary::unEffectHealth(AC_BaseCharacter* TargetActor, float Value)
{
	if (!TargetActor)
		return;
	float fHp = TargetActor->getHp();
	fHp = FMath::Clamp(fHp, TNumericLimits<float>::Min() + Value, fHp);
	fHp -= Value;
	TargetActor->setHp(fHp);

	fHp = TargetActor->getMaxHp();
	fHp = FMath::Clamp(fHp, TNumericLimits<float>::Min() + Value, fHp);
	fHp -= Value;
	TargetActor->setMaxHp(fHp);
}