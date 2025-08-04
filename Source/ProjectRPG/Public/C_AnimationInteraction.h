#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_AnimationInteraction.generated.h"

class UAnimMontage;
class UC_InteractionComponent;
class ACharacter;
class ATargetPoint;

UCLASS(BlueprintType, Blueprintable)
class PROJECTRPG_API AC_AnimationInteraction : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	UAnimMontage* m_pStartMontage{};
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	UAnimMontage* m_pLoopMontage{};
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	UAnimMontage* m_pEndMontage{};
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	float m_fLoopMoveSpeed = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	UC_InteractionComponent* m_pInteractionCollision{};
	USceneComponent* m_pRoot{};
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	ATargetPoint* m_pEndPoint;

	UPROPERTY(EditAnyWhere, Category = "AC_AnimationInteraction")
	float m_fLoopPlayTime{};
	
	FRotator m_sRotate{};
	FVector m_sDirection;
private:
	ACharacter* m_pDetector{};
	UAnimInstance* m_pInstance{};
	bool m_bLoopMove{};
	float m_fDistance;
public:	
	AC_AnimationInteraction();

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void playAnimation (AActor* pDetectedActor);

	void playLoopAnimation();
	void playEndAnimation();
	void resetAnimation();
};
