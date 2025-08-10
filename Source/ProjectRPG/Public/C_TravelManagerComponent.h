#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_TravelManagerComponent.generated.h"

UENUM(BlueprintType)
enum class E_TrabelType : uint8
{
	E_NONE				UMETA(DisplayName = "None"),
	E_ClimbUpStart		UMETA(DisplayName = "ClimbUpStart"),
	E_ClimbUpEnd		UMETA(DisplayName = "ClimbUpEnd"),
	E_ClimbDownStart	UMETA(DisplayName = "ClimbDownStart"),
	E_ClimbDownEnd		UMETA(DisplayName = "ClimbDownEnd"),
	E_SlopDownStart		UMETA(DisplayName = "SlopDownStart"),
	E_SlopDownEnd		UMETA(DisplayName = "SlopDownEnd"),
	E_ZipLineStart		UMETA(DisplayName = "ZipLineStart"),
	E_ZipLineEnd		UMETA(DisplayName = "ZipLineEnd"),
	E_BalanceBeamStart		UMETA(DisplayName = "BalanceBeamStart"),
	E_BalanceBeamEnd		UMETA(DisplayName = "BalanceBeamEnd"),
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRPG_API UC_TravelManagerComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	E_TrabelType m_eTravelType;
public:	
	UC_TravelManagerComponent();

	UFUNCTION(BlueprintPure)
	E_TrabelType getTravelType() const { return m_eTravelType; }
	UFUNCTION(BlueprintCallable)
	void setTravelType(E_TrabelType eType) { m_eTravelType = eType; }
protected:
	virtual void BeginPlay() override;
};
