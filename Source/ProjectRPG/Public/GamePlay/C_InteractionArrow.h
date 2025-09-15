#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "C_InteractionArrow.generated.h"

UCLASS(ClassGroup = "Interaction", meta = (DisplayName = "InteractionArrow", BlueprintSpawnableComponent))
class PROJECTRPG_API UC_InteractionArrow : public UStaticMeshComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "InteractionArrow")
	float m_fHieght = 80.f;
	UPROPERTY(EditDefaultsOnly, Category = "InteractionArrow")
	float m_fMoveSpeed = 60.f;
	UPROPERTY(EditDefaultsOnly, Category = "InteractionArrow")
	float m_fRotateSpeed = 160.f;

	float m_fPosition{};
	float m_fYaw{};
	float m_fDir = 1.0f;
public:
	UC_InteractionArrow(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void OnRegister() override;
};
