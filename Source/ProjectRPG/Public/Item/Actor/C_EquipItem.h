#pragma once

#include "CoreMinimal.h"
#include "C_ItemActorBase.h"
#include "C_EquipItem.generated.h"

class AC_BaseCharacter;



UCLASS()
class PROJECTRPG_API AC_EquipItem : public AC_ItemActorBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 m_nEquipID{};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 m_nEquipIndex{};
public:
	AC_EquipItem();

	int32 getEquipID() const { return m_nEquipID; }
	int32 getEquipIndex() const { return m_nEquipIndex; }

	void effectEquip(AC_BaseCharacter* pCharacter);
protected:
	virtual bool findActor_Implementation(AActor*& pTargetActor) override;
	virtual bool itemEffect_Implementation() override;
	virtual bool useFail_Implementation() override;
	virtual bool checkItemUseable_Implementation() override;
	virtual bool useSuccess_Implementation() override;

};
