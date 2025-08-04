#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "C_InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionEvent, AActor*, pDetectedActor);

UCLASS(ClassGroup = "Collision", meta = (DisplayName = "Interaction", BlueprintSpawnableComponent))
class PROJECTRPG_API UC_InteractionComponent : public UCapsuleComponent
{
	GENERATED_BODY()
protected:

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnInteractionEvent m_onInteraction{};

public:
	UC_InteractionComponent();

	UFUNCTION(BlueprintCallable)
	bool runInteraction(AActor* pDetectedActor);

	UFUNCTION(BlueprintPure)
	static FString getInteractionPresetName() { return TEXT("InteractionPreset"); }

	virtual void BeginPlay() override;
};
