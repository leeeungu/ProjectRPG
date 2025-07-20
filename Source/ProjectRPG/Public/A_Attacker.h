#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_Attacker.h"
#include "A_Attacker.generated.h"


UCLASS()
class TOPDOWNPORTFOLIO_API AA_Attacker : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Attacker, meta = (AllowPrivateAccess = "true"))
	USceneComponent* m_pRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attacker, meta = (AllowPrivateAccess = "true"))
	UC_Attacker* m_pAttacker;
public:	
	AA_Attacker();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;
public:	
	UFUNCTION(BlueprintCallable)
	void E_Init(AController* pInstigator);

	UFUNCTION(BlueprintCallable)
	void E_StartAttack(float fDamage, TSubclassOf<UDamageType> sDamageType = nullptr);
	UFUNCTION(BlueprintCallable)
	void E_EndAttack();
};
