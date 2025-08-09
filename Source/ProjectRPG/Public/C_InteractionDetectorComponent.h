#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "C_InteractionDetectorComponent.generated.h"

class UC_InteractionComponent;

UCLASS(ClassGroup = "Collision", meta = (DisplayName = "Detector", BlueprintSpawnableComponent))
class PROJECTRPG_API UC_InteractionDetectorComponent : public UCapsuleComponent
{
	GENERATED_BODY()
private:
	TSet< UC_InteractionComponent*> m_setInteractionComponents{};

	UC_InteractionComponent* m_pFirst{};
public:
	UC_InteractionDetectorComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure)
	static FString getDetectorPresetName() { return TEXT("DetectorPreset"); }


	UFUNCTION(BlueprintCallable)
	bool runInteraction();

	UFUNCTION(BlueprintPure)
	AActor* getInteractionActor() const;

	UFUNCTION(BlueprintPure)
	UC_InteractionComponent* getInteractionComponent();

private:
	UFUNCTION()
	void beginDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void endDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
