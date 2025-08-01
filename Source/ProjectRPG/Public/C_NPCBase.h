#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_SearchingInterface.h"
#include "C_NPCBase.generated.h"

class UC_SearchingCollision;

UCLASS(Blueprintable)
class PROJECTRPG_API AC_NPCBase : public ACharacter, public IC_SearchingInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AC_NPCBase")
	UC_SearchingCollision* m_pSearchCollision{};
public:
	AC_NPCBase();

//	bool beginSearchingEvent(AActor* pOther) override;
//	bool endSearchingEvent(AActor* pOther)override;

	virtual bool beginSearchingEvent_Implementation(AActor* pOther) override;
	virtual bool endSearchingEvent_Implementation(AActor* pOther) override;
protected:
	virtual void BeginPlay() override;


	//UFUNCTION()
	//void beginSearching(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//UFUNCTION()
	//void endSearching(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
