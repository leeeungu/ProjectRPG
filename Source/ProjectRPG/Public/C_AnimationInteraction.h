#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_AnimationInteraction.generated.h"

class UAnimMontage;
class UC_InteractionComponent;
class ACharacter;
class USkeletalMeshComponent;
class ATargetPoint;
class UArrowComponent;

USTRUCT(BlueprintType)
struct FS_InteractionAnimationData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "FS_InteractionAnimationData")

	UAnimMontage* pPlayMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FS_InteractionAnimationData")
	FVector startLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FS_InteractionAnimationData")
	FVector endLocation;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FS_InteractionAnimationData")
	float fMoveSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FS_InteractionAnimationData")
	float fNextMontageTime;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FS_InteractionAnimationData")
	float fDistance;
};

UCLASS(BlueprintType, Blueprintable)
class PROJECTRPG_API AC_AnimationInteraction : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	USceneComponent* m_pRoot{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	UArrowComponent* m_pStartDirection;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	UC_InteractionComponent* m_pInteractionCollision{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AC_AnimationInteraction")
	ATargetPoint* m_pEndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AC_AnimationInteraction")
	FS_InteractionAnimationData m_sStartAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AC_AnimationInteraction")
	FS_InteractionAnimationData m_sLoopAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AC_AnimationInteraction")
	FS_InteractionAnimationData m_sEndAnimation;

private:
	ACharacter* m_pDetector{};
	UAnimInstance* m_pInstance{};

public:	
	AC_AnimationInteraction();

	virtual void Tick(float DeltaTime) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void playAnimation (AActor* pDetectedActor);

	void playLoopAnimation();
	void playEndAnimation();
	void resetAnimation();
	void setAnimationData(FS_InteractionAnimationData* pData);
};
