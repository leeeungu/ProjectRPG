#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "C_GuidArrowComponent.generated.h"


UCLASS(ClassGroup = "GuidArrow", meta = (DisplayName = "GuidArrow", BlueprintSpawnableComponent))
class PROJECTRPG_API UC_GuidArrowComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GuidArrow")
	float m_fRotateSpeed = 30.0f;
	UPROPERTY(VisibleAnywhere, Category = "GuidArrow")
	FVector m_vTargetPosition;
public:
	UC_GuidArrowComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void startGuiding(FVector target);
	UFUNCTION(BlueprintCallable)
	void setTargetPosition(FVector target) { m_vTargetPosition = target; }
	UFUNCTION(BlueprintCallable)
	void endGuiding();
protected:
	virtual void OnRegister() override;
};
