#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_TravelManagerComponent.h"
#include "C_AnimationInteraction.generated.h"

class UAnimMontage;
class UC_InteractionComponent;
class ACharacter;
class USkeletalMeshComponent;
class ATargetPoint;
class UArrowComponent;

UCLASS(BlueprintType, Blueprintable)
class PROJECTRPG_API AC_AnimationInteraction : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	USceneComponent* m_pRoot{};
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	UArrowComponent* m_pStartDirection{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	UC_InteractionComponent* m_pStartCollision{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	UC_InteractionComponent* m_pEndCollision{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AC_AnimationInteraction")
	E_TrabelType m_eStartType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AC_AnimationInteraction")
	E_TrabelType m_eEndType{};
private:
	ACharacter* m_pDetector{};
	UC_TravelManagerComponent* m_pTravelManagerComponent{};
	bool m_bPlay{};
public:	
	AC_AnimationInteraction();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void playAnimation (AActor* pDetectedActor);

	UFUNCTION()
	void beginEndCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
