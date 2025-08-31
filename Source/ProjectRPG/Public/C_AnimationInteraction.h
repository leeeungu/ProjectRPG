#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_TravelManagerComponent.h"
#include "C_AnimationInteraction.generated.h"

class UWidgetComponent;
class UC_InteractionComponent;
class UCapsuleComponent;
class ACharacter;
class UArrowComponent;
//enum class E_TrabelType : uint8;

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
	UCapsuleComponent* m_pEndCollision2{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	UWidgetComponent* m_pInteractionWidget{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AC_AnimationInteraction")
	E_TrabelType m_eStartType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AC_AnimationInteraction")
	E_TrabelType m_eEndType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AC_AnimationInteraction")
	bool m_bRotateYaw{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AC_AnimationInteraction")
	bool m_bRotatePitch{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AC_AnimationInteraction")
	bool m_bRotateRoll{};

	FVector m_TargetLocations{};
private:
	ACharacter* m_pDetector{};
	UC_TravelManagerComponent* m_pTravelManagerComponent{};
	bool m_bPlay{};
public:	
	AC_AnimationInteraction();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void interactionStart (AActor* pDetectedActor);

	void StartAnimation();

	UFUNCTION()
	void beginEndCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void beginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void endOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void rotateToTarget();
};
