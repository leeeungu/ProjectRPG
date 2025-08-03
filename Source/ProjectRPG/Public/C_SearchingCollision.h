#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "C_SearchingInterface.h"
#include "C_DetectedInterface.h"
#include "C_SearchingCollision.generated.h"



UCLASS(ClassGroup = "Collision", meta = (DisplayName = "UC_SearchingCollision", BlueprintSpawnableComponent), MinimalAPI)
class UC_SearchingCollision : public UCapsuleComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "UC_SearchingCollision")
	TScriptInterface<IC_SearchingInterface> m_ownerInterface{};
	TSet< TScriptInterface<IC_DetectedInterface>> m_setDetected{};
public:
	UC_SearchingCollision();
	
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	UFUNCTION()
	void beginSearching(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void endSearching(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
